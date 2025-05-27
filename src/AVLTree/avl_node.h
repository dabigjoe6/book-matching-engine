#ifndef AVL_NODE_HPP
#define AVL_NODE_HPP

template <typename Derived, typename T>
class Node {
protected:
  explicit Node(T _value, Derived* _right_child = nullptr, Derived* _left_child = nullptr, Derived* _parent = nullptr): 
              value(_value), right_child(_right_child), left_child(_left_child), parent(_parent) {}

  T value;
  Derived* right_child;
  Derived* left_child;
  Derived* parent;

  int height = 0;

  friend Derived;
  friend AvlTree<Derived>;

public:
  Derived* get_right_child() const { return right_child; }
  Derived* get_left_child() const { return left_child; }
  Derived* get_parent() const { return parent; }

  Derived* get_height() const { return height; }

  friend bool operator>(const Node& lhs, const Node& rhs) {
    return static_cast<const Derived&>(lhs).value > static_cast<const Derived&>(rhs).value;
  }

  friend bool operator<(const Node& lhs, const Node& rhs) {
    return static_cast<const Derived&>(lhs).value < static_cast<const Derived&>(rhs).value;
  }

  friend bool operator==(const Node& lhs, const Node& rhs) {
    return static_cast<const Derived&>(lhs).value == static_cast<const Derived&>(rhs).value;
  }

  ~Node() = default;
};

#endif
