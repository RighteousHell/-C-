#include "gtest/gtest.h"
#include "../Storage/Storage.h"

TEST(StorageTest, TestCase1) 
{
	Storage storage_examp(10);
	EXPECT_EQ("", storage_examp.get("a"));
	EXPECT_NE("val_1", storage_examp.get("a"));
}
TEST(StorageTest, TestCase2)
{
	Storage storage_examp(2);

	storage_examp.put("a", "val_1");
	storage_examp.put("b", "val_2");

	storage_examp.get("a");
	storage_examp.put("c", "val_3");
	EXPECT_EQ("val_1", storage_examp.get("a"));
	EXPECT_EQ("", storage_examp.get("b"));
	EXPECT_EQ("val_3", storage_examp.get("c"));
}

TEST(StorageTest, TestCase3)
{
	Storage storage_examp(2);

	storage_examp.put("a", "val_1");
	storage_examp.put("b", "val_2");


	storage_examp.put("a", "val_1_2");
	storage_examp.put("c", "val_3");


	storage_examp.get("a");
	storage_examp.put("c", "val_3");
	EXPECT_EQ("val_1_2", storage_examp.get("a"));
	EXPECT_EQ("", storage_examp.get("b"));
	EXPECT_EQ("val_3", storage_examp.get("c"));
}