#include <gtest/gtest.h>

#include "../src/Limit/limit.h";

class LimitTests : public testing::Test {
public:
  LimitTests() {}

 virtual void SetUp() override {}

  virtual void TearDown() override {}

  Limit* limit;
};

TEST_F(LimitTests, TestLimitCreated) { EXPECT_EQ(limit, nullptr); }

TEST_F(LimitTests, TestLimitGetPrice) {
  OrderBook* orderBook = new OrderBook();
  int price = 100;
  LimitType type = LimitType.LimitBuy;
  
  limit = new Limit(orderBook, price, type);

  EXPECT_EQ(limit->getPrice(), price);
}
