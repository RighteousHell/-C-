#include "gtest/gtest.h"
#include "../Filter/filter.h"


TEST(Filter, TestCase1) 
{
  std::vector<int> preset = { 1, 0, 2, 9, -6, 1, -6, 0, 19, 5, 2 };
  std::vector<int> expected = { 1, 0, 2, -6 };
  std::vector<int> answer;

  
  answer = Filter::findItems(preset);

  std::sort(expected.begin(), expected.end());
  std::sort(answer.begin(), answer.end());


  EXPECT_EQ(expected, answer ); //positive

  expected.emplace_back(300);
  EXPECT_NE(expected, answer); //negative

}

