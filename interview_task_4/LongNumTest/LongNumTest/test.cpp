#include "pch.h"
#include "LongNumsSum.h"

TEST(BigNumTest, BothEmpty)
{

	big_num::Big_num big_num1(std::forward<std::string>(""));
	big_num::Big_num big_num2(std::forward<std::string>(""));
	big_num1 = big_num1 + big_num2;
	EXPECT_EQ(std::string(big_num1), "0");
	EXPECT_NE(std::string(big_num1), "1");
}
TEST(BigNumTest, BothZero)
{

	big_num::Big_num big_num1(std::forward<std::string>("0"));
	big_num::Big_num big_num2(std::forward<std::string>("0"));
	big_num1 = big_num1 + big_num2;
	EXPECT_EQ(std::string(big_num1), "0");
	EXPECT_NE(std::string(big_num1), "1");
}
TEST(BigNumTest, SmallNums_BothPos)
{

	big_num::Big_num big_num1(std::forward<std::string>("5782"));
	big_num::Big_num big_num2(std::forward<std::string>("1012"));
	big_num1 = big_num1 + big_num2;
	EXPECT_EQ(std::string(big_num1), "6794");
	EXPECT_NE(std::string(big_num1), "1");
}
TEST(BigNumTest, SmallNums_BothNeg)
{

	big_num::Big_num big_num1(std::forward<std::string>("-5782"));
	big_num::Big_num big_num2(std::forward<std::string>("-1012"));
	big_num1 = big_num1 + big_num2;
	EXPECT_EQ(std::string(big_num1), "-6794");
	EXPECT_NE(std::string(big_num1), "1");
}

TEST(BigNumTest, ZeroSum)
{

	big_num::Big_num big_num1(std::forward<std::string>("-5782"));
	big_num::Big_num big_num2(std::forward<std::string>("5782"));
	big_num1 = big_num1 + big_num2;
	EXPECT_EQ(std::string(big_num1), "0");
	EXPECT_NE(std::string(big_num1), "1");
}
TEST(BigNumTest, SeveralRlyBigNums)
{

	big_num::Big_num big_num1(std::forward<std::string>("--100000000015156568517886451"));
	big_num::Big_num big_num2(std::forward<std::string>("100000000015156568517886459548"));
	big_num::Big_num big_num3(std::forward<std::string>("5145542548248525"));
	big_num::Big_num big_num4(std::forward<std::string>("-845154874864845523"));
	big_num::Big_num big_num5(std::forward<std::string>("12314561565202308949964965264167676004871854615641564614608948680450456085450884824994985846276263746267660760679272"));
	big_num1 = big_num1 + big_num2;
	big_num1 = big_num1 + big_num3;
	big_num1 = big_num1 + big_num4;
	big_num1 = big_num1 + big_num5;
	EXPECT_EQ(std::string(big_num1), "12314561565202308949964965264167676004871854615641564614608948680450456085450884824995085746276278047670277812655371");
	EXPECT_NE(std::string(big_num1), "0");
}