template <typename Derived, typename T>
class Node {
protected:
  explicit Node(T _value, Derived* _right_child, Derived* _left_child, Derived* _parent);

  T value;
  Derived* right_child;
  Derived* left_child;
  Derived* parent;

  int height;

  friend Derived;
  friend AvlTree;

public:
  Derived* get_right_child() const { return right_child; }
  Derived* get_left_child() const { return left_child; }
  Derived* get_parent() const { return parent; }

  Derived* get_height() const { return height; }

  friend bool operator>(Derived *lhs, Derived *rhs) const {
    return lhs->value > rhs->value;
  }

  friend bool operator<(Derived* lhs, Derived* rhs) const {
    return lhs->value < rhs->value;
  }

  friend bool operator==(Derived* lhs, Derived* rhs) const {
    return lhs->value == rhs->value;
  }

  ~Node() = default;
};
