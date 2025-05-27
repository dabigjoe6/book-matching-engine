#ifndef AVL_TREE_INL_H
#define AVL_TREE_INL_H

#include <algorithm>

template<typename T>
T* AvlTree<T>::_insert(T* root, T* node, T* parent) {
 if (root == nullptr) {
    node->parent = parent;
    return node; 
  }

  if (*node < *root) {
    root->left_child = _insert(root->left_child, node, root);
  } else if (*node > *root) {
    root->right_child = _insert(root->right_child, node, root);
  } else {
    return root; // no 2 nodes with the same value
  }

  update_height(root);
  return balance_node(root);
}

template<typename T>
T* AvlTree<T>::_delete(T*& root, T* node, T* parent) {
  if (root == nullptr) {
    return root;
  }

  if (*node < *root) {
    root->left_child = _delete(root->left_child, node, root);
  } else if (*node > *root) {
    root->right_child = _delete(root->right_child, node, root);
  } else {
    if (root->left_child == nullptr) {
      T* temp = root->right_child;
      if (temp != nullptr) {
        temp->parent = parent;
      }
      delete root;
      root = temp;
      return root;
    } 

    if (root->right_child == nullptr) {
      T* temp = root->left_child;
      if (temp != nullptr) {
        temp->parent = parent;
      }
      delete root;
      root = temp;
      return root;
    }

    T* temp = get_min_value_node(root->right_child);
    root->value = temp->value;
    root->right_child = _delete(root->right_child, temp, root);
  }

  update_height(root);
  return balance_node(root);
}

template<typename T>
T* AvlTree<T>::rotate_left(T* node) {
  T* new_node = node->right_child;
  node->right_child = new_node->left_child;
  if (new_node->left_child != nullptr) {
    new_node->left_child->parent = node;
  }

  new_node->left_child = node;
  new_node->parent = node->parent;
  node->parent = new_node;

  update_height(node);
  update_height(new_node);

  return new_node;
}

template<typename T>
T* AvlTree<T>::rotate_right(T* node) {
  T* new_node = node->left_child;
  node->left_child = new_node->right_child;
  if (new_node->right_child != nullptr) {
    new_node->right_child->parent = node;
  }

  new_node->right_child = node;
  new_node->parent = node->parent;
  node->parent = new_node;

  update_height(node);
  update_height(new_node);
  
  return new_node;
}

template<typename T>
T* AvlTree<T>::balance_node(T* node) {
  int balance = get_balance(node);

  // Left Left case
  if (balance > 1 && get_balance(node->left_child) >= 0) {
    return rotate_right(node);
  }

  // Left Right case
  if (balance > 1 && get_balance(node->left_child) < 0) {
    node->left_child = rotate_left(node->left_child);
    return rotate_right(node);
  }

  // Right Right case
  if (balance < -1 && get_balance(node->right_child) <= 0) {
    return rotate_left(node);
  }

  // Right Left case
  if (balance < -1 && get_balance(node->right_child) > 0) {
    node->right_child = rotate_right(node->right_child);
    return rotate_left(node);
  }

  return node;
}

template<typename T>
T* AvlTree<T>::get_min_value_node(T* node) {
  T* current_node = node;
  while (current_node->left_child != nullptr) {
    current_node = current_node->left_child;
  }

  return current_node;
}

template<typename T>
void AvlTree<T>::update_height(T* node) {
  node->height = 1 + std::max(get_height(node->right_child), get_height(node->left_child));
}

template<typename T>
int AvlTree<T>::get_height(T* node) {
  if (node == nullptr) {
    return -1;
  }

  return node->height;
}

template<typename T>
int AvlTree<T>::get_balance(T* node) {
  if (node == nullptr) {
    return 0;
  }

  return get_height(node->left_child) - get_height(node->right_child);
}

#endif
