#include <gtest/gtest.h>

class LimitTests : public testing::test {
public:
  LimitTests() {}

  virtual void SetUp override {}

  virtual void TearDown override {}
};

TEST_F(LimitTests, HelloWorldTest) { EXPECT_EQ(1, 1); }
