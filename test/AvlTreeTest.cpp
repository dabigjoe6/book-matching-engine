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
/*
*                                                  15
*                10                               /  \
*                  \                            /      \
*                  20           --->           10      20
*                 / 
*                15
*
*/

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

  // Right-Left rotation
  EXPECT_EQ(tree->get_root()->get_value(), 15);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 20);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 10);

  EXPECT_EQ(tree->get_root()->get_height(), 1);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);
}

TEST_F(AvlTreeTests, AvlTreeLeftRightRotation) {
/*
*                                                 9
*                10                              /  \
*               /                              /      \
*              8           --->               8       10
*               \   
*                9
*
*/

  LimitType limitType = LimitType::LimitBuy;

  Limit* node1 = new Limit(10, limitType);
  AvlTree<Limit>* tree = new AvlTree<Limit>(node1);

  EXPECT_EQ(tree->get_root()->get_value(), 10);

  Limit* node2 = new Limit(8, limitType);
  tree->insert_node(node2);

  EXPECT_EQ(tree->get_root()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_height(), 1);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);

  Limit* node3 = new Limit(9, limitType);
  tree->insert_node(node3);

  // Left-Right rotation
  EXPECT_EQ(tree->get_root()->get_value(), 9);
  EXPECT_EQ(tree->get_root()->get_height(), 1);

  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);

  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);
}

TEST_F(AvlTreeTests, AvlTreeRightRotation) {
/*
*                                                 8
*                10                              /  \
*               /                              /      \
*              8           --->               7       10
*             /
*            7
*
*/

  LimitType limitType = LimitType::LimitBuy;

  Limit* node1 = new Limit(10, limitType);
  AvlTree<Limit>* tree = new AvlTree<Limit>(node1);

  EXPECT_EQ(tree->get_root()->get_value(), 10);

  Limit* node2 = new Limit(8, limitType);
  tree->insert_node(node2);

  EXPECT_EQ(tree->get_root()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_height(), 1);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);

  Limit* node3 = new Limit(7, limitType);
  tree->insert_node(node3);

  // Right rotation
  EXPECT_EQ(tree->get_root()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_height(), 1);

  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 7);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);

  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);
}


TEST_F(AvlTreeTests, AvlTreeLeftRotation) {
/*
*                                                 15
*                10                              /  \
*                  \                           /      \
*                   15      --->              10       20
*                     \
*                      20
*/

  LimitType limitType = LimitType::LimitBuy;

  Limit* node1 = new Limit(10, limitType);
  AvlTree<Limit>* tree = new AvlTree<Limit>(node1);

  EXPECT_EQ(tree->get_root()->get_value(), 10);

  Limit* node2 = new Limit(15, limitType);
  tree->insert_node(node2);

  EXPECT_EQ(tree->get_root()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_height(), 1);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 15);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);

  Limit* node3 = new Limit(20, limitType);
  tree->insert_node(node3);

  // Left rotation
  EXPECT_EQ(tree->get_root()->get_value(), 15);
  EXPECT_EQ(tree->get_root()->get_height(), 1);

  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 10);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_height(), 0);

  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 20);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_height(), 0);
}

TEST_F(AvlTreeTests, AvlTreeDeletingNodeWithOnlyLeftChild) {
/*
*                8                              8
*               /  \     Deleting node 7      /   \            
*              7   10       --->            6     10
*             /
*            6
*/

  LimitType limitType = LimitType::LimitBuy;

  Limit* nodeA = new Limit(10, limitType);
  Limit* nodeB = new Limit(8, limitType);
  Limit* nodeC = new Limit(7, limitType);
  Limit* nodeD = new Limit(6, limitType);

  AvlTree<Limit>* tree = new AvlTree<Limit>(nodeA);

  tree->insert_node(nodeB);
  tree->insert_node(nodeC);
  tree->insert_node(nodeD);

  EXPECT_EQ(tree->get_root()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 7);

  tree->delete_node(nodeC);

  EXPECT_EQ(tree->get_root()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_left_child()->get_value(), 6);
}

TEST_F(AvlTreeTests, AvlTreeDeletingNodeWithOnlyRightChild) {
/*
*                8                             8
*               /  \     Deleting node 10    /   \            
*              7   10       --->            7     12
*                    \
*                    12
*
*/

  LimitType limitType = LimitType::LimitBuy;

  Limit* nodeA = new Limit(10, limitType);
  Limit* nodeB = new Limit(8, limitType);
  Limit* nodeC = new Limit(7, limitType);
  Limit* nodeD = new Limit(12, limitType);

  AvlTree<Limit>* tree = new AvlTree<Limit>(nodeA);

  tree->insert_node(nodeB);
  tree->insert_node(nodeC);
  tree->insert_node(nodeD);

  EXPECT_EQ(tree->get_root()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 10);

  tree->delete_node(nodeA);

  EXPECT_EQ(tree->get_root()->get_value(), 8);
  EXPECT_EQ(tree->get_root()->get_right_child()->get_value(), 12);
}
