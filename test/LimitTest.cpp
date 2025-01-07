#include <gtest/gtest.h>
#include <iostream>

class LimitTests : public testing::Test {
public:
  LimitTests() {}

  virtual void SetUp() override { std::cout << "Hello SetUp" << "\n"; }

  virtual void TearDown() override { std::cout << "Hello TearDown" << "\n"; }
};

TEST_F(LimitTests, HelloWorldTest) { EXPECT_EQ(1, 1); }
