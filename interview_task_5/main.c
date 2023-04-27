#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/bio.h>
#include <linux/bvec.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/numa.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/moduleparam.h>
#include <linux/spinlock_types.h>

#define SBDD_SECTOR_SHIFT      9
#define SBDD_SECTOR_SIZE       (1 << SBDD_SECTOR_SHIFT)
#define SBDD_MIB_SECTORS       (1 << (20 - SBDD_SECTOR_SHIFT))
#define SBDD_NAME              "sbdd"
#define MY_IOCTL_OPEN_DEV      _IOR('A', 1, char*)  
struct sbdd {
	wait_queue_head_t       exitwait;
	spinlock_t              datalock;
	atomic_t                deleting;
	atomic_t                refs_cnt;
	sector_t                capacity;
	u8                      *data;
	struct gendisk          *gd;
	struct request_queue    *q;
	struct block_device *phy_dev;
};

static struct sbdd      __sbdd;
static int              __sbdd_major = 0;
static unsigned long    __sbdd_capacity_mib = 100;
static mode_t __phy_mode = FMODE_READ | FMODE_WRITE;
static atomic_t __phy_dev_flag = ATOMIC_INIT(0);
static char *__phy_dev_name = SBDD_NAME;
static struct bio_set __sbdd_bio_set;

static int plug_physical_device(char * path);


static void phy_end_io(struct bio *bio)
{
    struct bio *orig_bio = bio->bi_private;

    orig_bio->bi_status = bio->bi_status;	
    bio_put(bio);
	pr_debug("Clone processed\n");

	
    if (atomic_dec_and_test(&__sbdd.refs_cnt)) {
		wake_up(&__sbdd.exitwait);
    }

	bio_endio(orig_bio);
	pr_debug("Original bio processed\n");
    
}

static int sbdd_ioctl(struct block_device *bdev, fmode_t mode, unsigned int cmd, unsigned long arg)
{
    int ret = 0;

    switch (cmd) {
        case MY_IOCTL_OPEN_DEV:
            {
                char path[NAME_MAX + 1];
                if (copy_from_user(&path, (char __user *)arg, NAME_MAX)) {
                    ret = -ENOENT;
					pr_err("can't to get path from user buf\n");
                    break;
                }
				path[NAME_MAX] = '\0';
				plug_physical_device(path);
            }
            break;
        default:
            ret = -ENOTTY;
    }

    return ret;
}

static sector_t sbdd_xfer(struct bio_vec* bvec, sector_t pos, int dir)
{
	void *buff = page_address(bvec->bv_page) + bvec->bv_offset;
	sector_t len = bvec->bv_len >> SBDD_SECTOR_SHIFT;
	size_t offset;
	size_t nbytes;

	if (pos + len > __sbdd.capacity)
		len = __sbdd.capacity - pos;

	offset = pos << SBDD_SECTOR_SHIFT;
	nbytes = len << SBDD_SECTOR_SHIFT;

	spin_lock(&__sbdd.datalock);

	if (dir)
		memcpy(__sbdd.data + offset, buff, nbytes);
	else
		memcpy(buff, __sbdd.data + offset, nbytes);

	spin_unlock(&__sbdd.datalock);

	pr_debug("pos=%6llu len=%4llu %s\n", pos, len, dir ? "written" : "read");

	return len;
}

static void sbdd_xfer_bio(struct bio *bio)
{
	struct bio *orig_bio = bio;

	if (likely(atomic_read(&__phy_dev_flag))) {
		struct bio *clone_bio = bio_clone_fast(bio, GFP_KERNEL, &__sbdd_bio_set);
		if (!clone_bio) {
		pr_err("unable to clone bio\n");
		return;
	}
		clone_bio->bi_end_io = phy_end_io;
		clone_bio->bi_opf |= REQ_PREFLUSH | REQ_FUA;
		clone_bio->bi_private = orig_bio;
		bio_set_dev(clone_bio, __sbdd.phy_dev);
		submit_bio(clone_bio);

	}
	else {
	struct bvec_iter iter;
	struct bio_vec bvec;
	int dir = bio_data_dir(bio);
	sector_t pos = bio->bi_iter.bi_sector;

	bio_for_each_segment(bvec, bio, iter)
		pos += sbdd_xfer(&bvec, pos, dir);

	bio_endio(bio);
	if (atomic_dec_and_test(&__sbdd.refs_cnt))
		wake_up(&__sbdd.exitwait);
	}
}

static blk_qc_t sbdd_make_request(struct request_queue *q, struct bio *bio)
{
	if (atomic_read(&__sbdd.deleting)) {
		pr_err("unable to process bio while deleting\n");
		bio_io_error(bio);
		return BLK_STS_IOERR;
	}
	atomic_inc(&__sbdd.refs_cnt);

	sbdd_xfer_bio(bio);
	return BLK_STS_OK;
}

static int plug_physical_device(char * path)
{
	int ret = 0;

	if (bioset_init(&__sbdd_bio_set, BIO_POOL_SIZE, 0, 0)) {
		pr_err("can't to init bioset\n");
		return -EPERM;	
	}
	if (atomic_read(&__phy_dev_flag)) {
		pr_err("physical device alredy plugged\n");
		return -EBUSY;
	}
	__sbdd.phy_dev = blkdev_get_by_path(path, __phy_mode, THIS_MODULE);
        if (IS_ERR(__sbdd.phy_dev)) {
            ret = PTR_ERR(__sbdd.phy_dev);
            pr_err("can't to open phy_dev by path: %s\n", path);
        }
		else {
			pr_info("phy device plugged successfully");
			__sbdd.capacity = get_capacity(__sbdd.phy_dev->bd_disk);
			set_capacity(__sbdd.gd, __sbdd.capacity);
			atomic_inc(&__phy_dev_flag);
		}
	return ret;
}

/*
There are no read or write operations. These operations are performed by
the request() function associated with the request queue of the disk.
*/
static struct block_device_operations const __sbdd_bdev_ops = {
	.owner = THIS_MODULE,
	.ioctl = sbdd_ioctl,
};

static int sbdd_create(void)
{
	int ret = 0;

	/*
	This call is somewhat redundant, but used anyways by tradition.
	The number is to be displayed in /proc/devices (0 for auto).
	*/
	pr_info("registering blkdev\n");
	__sbdd_major = register_blkdev(0, SBDD_NAME);
	if (__sbdd_major < 0) {
		pr_err("call register_blkdev() failed with %d\n", __sbdd_major);
		return -EBUSY;
	}

	memset(&__sbdd, 0, sizeof(struct sbdd));
	__sbdd.capacity = (sector_t)__sbdd_capacity_mib * SBDD_MIB_SECTORS;

	pr_info("allocating data\n");
	__sbdd.data = vzalloc(__sbdd.capacity << SBDD_SECTOR_SHIFT);
	if (!__sbdd.data) {
		pr_err("unable to alloc data\n");
		return -ENOMEM;
	}

	spin_lock_init(&__sbdd.datalock);
	init_waitqueue_head(&__sbdd.exitwait);

	pr_info("allocating queue\n");
	__sbdd.q = blk_alloc_queue(GFP_KERNEL);
	if (!__sbdd.q) {
		pr_err("call blk_alloc_queue() failed\n");
		return -EINVAL;
	}
	blk_queue_make_request(__sbdd.q, sbdd_make_request);

	/* Configure queue */
	blk_queue_logical_block_size(__sbdd.q, SBDD_SECTOR_SIZE);

	/* A disk must have at least one minor */
	pr_info("allocating disk\n");
	__sbdd.gd = alloc_disk(1);

	/* Configure gendisk */
	__sbdd.gd->queue = __sbdd.q;
	__sbdd.gd->major = __sbdd_major;
	__sbdd.gd->first_minor = 0;
	__sbdd.gd->fops = &__sbdd_bdev_ops;
	/* Represents name in /proc/partitions and /sys/block */
	scnprintf(__sbdd.gd->disk_name, DISK_NAME_LEN, SBDD_NAME);
	set_capacity(__sbdd.gd, __sbdd.capacity);

	if (0 != strcmp(__phy_dev_name, SBDD_NAME) && strlen(__phy_dev_name) < NAME_MAX) {
		plug_physical_device(__phy_dev_name);
	}
	
	/*
	Allocating gd does not make it available, add_disk() required.
	After this call, gd methods can be called at any time. Should not be
	called before the driver is fully initialized and ready to process reqs.
	*/
	pr_info("adding disk\n");
	add_disk(__sbdd.gd);

	return ret;
}

static void sbdd_delete(void)
{
	atomic_set(&__sbdd.deleting, 1);

	wait_event(__sbdd.exitwait, !atomic_read(&__sbdd.refs_cnt));
	
	if (atomic_read(&__phy_dev_flag)) {
		pr_info("Put phy decice\n");
		bioset_exit(&__sbdd_bio_set);
		blkdev_put(__sbdd.phy_dev, __phy_mode);
	}

	/* gd will be removed only after the last reference put */
	if (__sbdd.gd) {
		pr_info("deleting disk\n");
		del_gendisk(__sbdd.gd);
	}

	if (__sbdd.q) {
		pr_info("cleaning up queue\n");
		blk_cleanup_queue(__sbdd.q);
	}

	if (__sbdd.gd)
		put_disk(__sbdd.gd);

	if (__sbdd.data) {
		pr_info("freeing data\n");
		vfree(__sbdd.data);
	}

	memset(&__sbdd, 0, sizeof(struct sbdd));

	if (__sbdd_major > 0) {
		pr_info("unregistering blkdev\n");
		unregister_blkdev(__sbdd_major, SBDD_NAME);
		__sbdd_major = 0;
	}
}

/*
Note __init is for the kernel to drop this function after
initialization complete making its memory available for other uses.
There is also __initdata note, same but used for variables.
*/
static int __init sbdd_init(void)
{
	int ret = 0;

	pr_info("starting initialization...\n");
	ret = sbdd_create();

	if (ret) {
		pr_warn("initialization failed\n");
		sbdd_delete();
	} else {
		pr_info("initialization complete\n");
	}

	return ret;
}

/*
Note __exit is for the compiler to place this code in a special ELF section.
Sometimes such functions are simply discarded (e.g. when module is built
directly into the kernel). There is also __exitdata note.
*/
static void __exit sbdd_exit(void)
{
	pr_info("exiting...\n");
	sbdd_delete();
	pr_info("exiting complete\n");
}

/* Called on module loading. Is mandatory. */
module_init(sbdd_init);

/* Called on module unloading. Unloading module is not allowed without it. */
module_exit(sbdd_exit);

/* Set desired capacity with insmod */
module_param_named(capacity_mib, __sbdd_capacity_mib, ulong, S_IRUGO);
module_param_named(phy_dev_name, __phy_dev_name, charp, 0644);
MODULE_PARM_DESC(phy_dev_name, "Path to phisycal device. For example: /dev/sda1");

/* Note for the kernel: a free license module. A warning will be outputted without it. */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Block Device Driver");
