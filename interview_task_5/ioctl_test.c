#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#define OPEN_PHY_DEV      _IOR('A', 1, char*)
#define MAX_NAME_DEV			20

int main()
{
    int fd, ret;
    char adress[MAX_NAME_DEV] = "/dev/sdb";

    fd = open("/dev/sbdd", O_RDWR);

    if (fd == -1) {
        perror("Failed to open the device");
        return 1;
    }

    ret = ioctl(fd, OPEN_PHY_DEV, adress);

    if (ret == -1) {
        perror("Failed to perform ioctl");
        close(fd);
        return 1;
    }

    close(fd);

    return 0;
}