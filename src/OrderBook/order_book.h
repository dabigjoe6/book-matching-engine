#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <unordered_map>

class Limit;
class Order;
class OrderGenerator;
class AvlTree;

enum class LimitType;

class OrderBook {
public:
  OrderBook();

  void add_order(Order *new_order);

  Limit *get_highest_buy();
  Limit *get_lowest_sell();

  Limit *get_highest_stop_sell();
  Limit *get_lowest_stop_buy();

  void delete_limit(Limit* limit, int buy_or_sell);
  void delete_stop_limit(Limit* limit, int buy_or_sell);

private:
  AvlTree<Limit> *buy_tree = nullptr;
  AvlTree<Limit> *sell_tree = nullptr;

  AvlTree<Limit> *stop_buy_tree = nullptr;
  AvlTree<Limit> *stop_sell_tree = nullptr;

  Limit *highest_buy;
  Limit *lowest_sell;

  Limit *highest_stop_sell;
  Limit *lowest_stop_buy;

  // TODO: Why did I do this? - have a pointer to the map?
  std::unordered_map<int, Limit *> _buy_limit_map = {};
  std::unordered_map<int, Limit *> *buy_limit_map = &_buy_limit_map;

  // TODO: Why did I do this? - have a pointer to the map?
  std::unordered_map<int, Limit *> _sell_limit_map = {};
  std::unordered_map<int, Limit *> *sell_limit_map = &_sell_limit_map;

  // TODO: Why did I do this? - have a pointer to the map?
  std::unordered_map<int, Limit *> _stop_buy_map = {};
  std::unordered_map<int, Limit *> *stop_buy_map = &_stop_buy_map;
  
  // TODO: Why did I do this? - have a pointer to the map?
  std::unordered_map<int, Limit *> _stop_sell_map = {};
  std::unordered_map<int, Limit *> *stop_sell_map = &_stop_sell_map;

  void place_order(Order *order);

  void place_market_order(Order *order);
  void place_limit_order(Order *order);
  void place_stop_order(Order *order);

  void add_limit_order(Order *order);
  void add_stop_order(Order *order);

  void remove_limit();
  void remove_stop_limit(Limit* limit, int buy_or_sell);

  int market_order_helper(Limit *limit, Order *order);
  bool add_stop_order_as_market_or_limit_order(Limit* edge_limit, Order* order);

  void execute_limit_order(Limit* edge, Order *order);

  void execute_stop_orders(int buy_or_sell);

  void update_book_edge_on_insert(Limit* new_limit, const LimitType& type);
  void update_book_edge_on_delete(Limit* limit, const LimitType& type);

  friend OrderGenerator;
};

#endif
