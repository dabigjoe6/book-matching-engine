#include <gtest/gtest.h>

#include "../src/AVLTree/avl_tree.h"
#include "../src/AVLTree/avl_node.h"

#include "../src/Limit/limit.h"

class AvlTreeTests : public testing::Test {
public:
  AvlTreeTests() {}

 virtual void SetUp() override {
  }

  virtual void TearDown() override {
    
  }
};

TEST_F(AvlTreeTests, AvlTreeCreated) {
  AvlTree<Limit>* tree = new AvlTree<Limit>();
  EXPECT_EQ(tree->get_root(), nullptr); 
}

TEST_F(AvlTreeTests, AvlTreeRootChild) {
  Limit* node = new Limit(10, LimitType::LimitBuy);
  AvlTree<Limit>* tree = new AvlTree<Limit>(node);

  EXPECT_EQ(tree->get_root()->get_height(), 0);
  EXPECT_EQ(tree->get_root()->get_parent(), nullptr);
  EXPECT_EQ(tree->get_root()->get_right_child(), nullptr);
  EXPECT_EQ(tree->get_root()->get_left_child(), nullptr);
  EXPECT_EQ(tree->get_root()->get_value(), 10);
}

TEST_F(AvlTreeTests, AvlTreeRightLeftRotation) {
  LimitType limitType = LimitType::LimitBuy;

  Limit* node1 = new Limit(10, limitType);
  AvlTree<Limit>* tree = new AvlTree<Limit>(node1);

  EXPECT_EQ(tree->get_root()->get_value(), 10);

  Limit* node2 = new Limit(20, limitType);
  tree->insert_node(node2);

  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 20);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);
  
  EXPECT_EQ(tree->get_root()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_height(), 1);

  Limit* node3 = new Limit(15, limitType);
  tree->insert_node(node3);

  EXPECT_EQ(tree->get_root()->get_value(), 15);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 20);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 10);

  // Limit* node4 = new Limit(18, limitType);
  // tree->insert_node(node4);
  //
  // EXPECT_EQ(tree->get_root()->get_value(), 10);
  // EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 18);
  // EXPECT_EQ(tree->get_root()->get_right_child()->get_left_child()->get_value(), 15);
  // EXPECT_EQ(tree->get_root()->get_right_child()->get_right_child()->get_value(), 20);

  // EXPECT_EQ(tree->get_root()->get_value(), 15);
  // EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 10);
  // EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 20);
  //
  // EXPECT_EQ(tree->get_root()->get_height(), 1);
  // EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);
  // EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);
}
