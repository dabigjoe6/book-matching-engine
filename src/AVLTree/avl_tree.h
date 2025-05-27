#ifndef AVL_TREE_H
#define AVL_TREE_H

template <typename T>
class AvlTree {
public:
  AvlTree(T* root): root(root) {}

  void insert_node(T* node) {
    _insert(root, node);
  }

  void delete_node(T* node) {
    _delete(root, node);
  }


private:
  T* root;

  T* _insert(T* root, T* node, T* parent = nullptr);
  T* _delete(T*& root, T* node, T* parent = nullptr);

  T* rotate_left(T* node);
  T* rotate_right(T* node);

  T* get_min_value_node(T* node);

  void update_height(T* node);
  int get_height(T* node);
  int get_balance(T* node);

  T* balance_node(T* node);
};

#include "./avl_tree-inl.h"

#endif
