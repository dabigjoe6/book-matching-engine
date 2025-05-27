#include <algorithm>
#include <iomanip>
#include <iostream>

#include "order_book.h"

#include "../Limit/limit.h"
#include "../Order/order.h"
#include "../AVLTree/avl_tree.h"
#include "../AVLTree/avl_node.h"

OrderBook::OrderBook() {}

Limit *OrderBook::get_highest_buy() { return highest_buy; }
Limit *OrderBook::get_lowest_sell() { return lowest_sell; }

Limit *OrderBook::get_lowest_stop_buy() { return lowest_stop_buy; }
Limit *OrderBook::get_highest_stop_sell() { return highest_stop_sell; }

void OrderBook::place_order(Order *new_order) {
  // market order
  // We assume for a market order the limit price and stop price are set to 0
  if (new_order->get_limit_price() == 0 && new_order->get_stop_price() == 0) {
    std::cout << std::left
              << "ID " << std::setw(8) << new_order->get_id()
              << std::setw(5) << (new_order->get_buy_or_sell() ? "Buy" : "Sell") << std::setw(30)
              << "[MARKET ORDER SUBMITTED]" << " for " << new_order->get_shares()
              << "\n";
    place_market_order(new_order);
    return;
  }

  // limit order
  // We assume for a limit order the stop price is set to 0
  if (new_order->get_limit_price() > 0 && new_order->get_stop_price() == 0) {
    std::cout << std::left 
              << "ID " << std::setw(8) << new_order->get_id()
              << std::setw(5) << (new_order->get_buy_or_sell() ? "Buy" : "Sell") << std::setw(30)
              << "[LIMIT ORDER SUBMITTED]" << " for " << new_order->get_shares()
              << " @ Limit Price: $" << new_order->get_limit_price() << "\n";
    place_limit_order(new_order);
    return;
  }

  // stop order or stop limit order
  if (new_order->get_stop_price() > 0) {
    if (new_order->get_stop_price() > 0 && new_order->get_limit_price() == 0) {
      std::cout << std::left
                << "ID " << std::setw(8) << new_order->get_id()
                << std::setw(5) << (new_order->get_buy_or_sell() ? "Buy" : "Sell") << std::setw(30)
                << "[STOP ORDER SUBMITTED]" << " for " << new_order->get_shares()
                << " @ Limit Price: $" << new_order->get_limit_price()
                << " @ Stop Price: $" << new_order->get_stop_price() << "\n";
    } else {
      std::cout << std::left
                << "ID " << std::setw(8) << new_order->get_id()
                << std::setw(5) << (new_order->get_buy_or_sell() ? "Buy" : "Sell") << std::setw(30)
                << "[STOP LIMIT ORDER SUBMITTED]" << " for " << new_order->get_shares()
                << " @ Limit Price: $" << new_order->get_limit_price()
                << " @ Stop Price: $" << new_order->get_stop_price() << "\n";
    }
    place_stop_order(new_order);
    return;
  }
}

void OrderBook::place_market_order(Order *order) {
  Limit *opposite_edge_limit =
      order->get_buy_or_sell() ? this->get_lowest_sell() : this->get_highest_buy();

  // If the order is partially filled or there's no existing order to match
  // against, we forget the order
  market_order_helper(opposite_edge_limit, order);

  execute_stop_orders(order->get_buy_or_sell());
}

void OrderBook::place_limit_order(Order *order) {
  Limit *opposite_edge_limit =
      order->get_buy_or_sell() ? this->get_lowest_sell() : this->get_highest_buy();

  int shares = market_order_helper(opposite_edge_limit, order);

  if (shares != 0) {
    add_limit_order(order);
  }

  execute_stop_orders(order->get_buy_or_sell());
}

void OrderBook::place_stop_order(Order *order) {
  Limit *opposite_edge_limit =
      order->get_buy_or_sell() ? this->get_lowest_sell() : this->get_highest_buy();

  if (!add_stop_order_as_market_or_limit_order(opposite_edge_limit, order)) {
    add_stop_order(order);
  }
}

// Incrementally fills order by matching the edge orders (opposite buy/sell
// side) returns remaining shares
int OrderBook::market_order_helper(Limit *edge, Order *order) {
  // TODO: What determines the price of the edge when limit price is 0
  // Do we just match all possible shares automatically? - What does the real
  // world scenario look like?
  while ((edge != nullptr && order != nullptr) &&
         ((order->get_limit_price() ==
               0 || // market order (we assume limit price is 0)
           (        // limit order to buy or sell
               (order->get_buy_or_sell() &&
                order->get_limit_price() >= edge->get_price()) ||
               (!order->get_buy_or_sell() &&
                order->get_limit_price() <= edge->get_price()))) &&
          (edge->get_head_order() != nullptr && order->get_shares() != 0))) {
    execute_limit_order(edge, order);
  }
  if (order != nullptr) {
    return order->get_shares();
  } else { return 0; }
}

void OrderBook::execute_limit_order(Limit* edge, Order*& order) {
  Order*& opposite_order = edge->head_order;
  if (opposite_order->get_shares() <= order->get_shares()) {
    int filled_shares = opposite_order->get_shares();

    // We will always deduct from volume and shares because we are 
    // simulating filled order regardless of buy/sell side
    edge->volume -= opposite_order->get_shares();

    order->set_shares(order->get_shares() - opposite_order->get_shares());
    order->set_filled_shares(filled_shares + order->get_filled_shares());

    std::cout << std::left
              << "ID " << std::setw(8) << order->get_id()
              << std::setw(5) << (order->get_buy_or_sell() ? "Buy" : "Sell") << std::setw(30)
              << (order->get_shares() == 0 ? "[FILL]" : "[PARTIAL FILL]")
              << " for " << filled_shares << " @ "
              << "Limit Price: $" << opposite_order->get_limit_price() << "\n";

    edge->head_order = edge->head_order->get_next_order();
    if (edge->head_order != nullptr) {
      edge->head_order->set_prev_order(nullptr);
    } else {
      remove_limit(edge, !order->get_buy_or_sell());
      
      // TODO: Need to record somewhere completelly filled orders before deleting the order instance
      delete opposite_order;
      opposite_order = nullptr;
    }

    return;
  }

  int filled_shares = order->get_shares();

  edge->volume -= order->get_shares();

  opposite_order->set_shares(opposite_order->get_shares() - order->get_shares());
  opposite_order->set_filled_shares(opposite_order->get_filled_shares() + order->get_shares());

  std::cout << std::left 
            << "ID " << std::setw(8) << order->get_id()
            << std::setw(5) << (order->get_buy_or_sell() ? "Buy" : "Sell") << std::setw(30)
            << "[FILL]" << " for " << filled_shares << " @ "
            << "Limit Price: $" << opposite_order->get_limit_price() << "\n";


  // TODO: Need to record somewhere completelly filled orders before deleting the order instance
  delete order;
  order = nullptr;

  return;
}

void OrderBook::execute_stop_orders(int buy_or_sell) {
  if (buy_or_sell) {
    while (lowest_stop_buy != nullptr &&
           (lowest_sell == nullptr ||
            lowest_stop_buy->get_price() >= lowest_sell->get_price())) {
      std::cout << std::left 
                << "ID " << std::setw(8) << lowest_stop_buy->get_head_order()->get_id()
                << std::setw(5) << "Buy" << std::setw(37)
                << "[STOP ORDER]" << " as Limit Order @ " << lowest_stop_buy->get_price() << "\n";
      add_limit_order(lowest_stop_buy->get_head_order());
      lowest_stop_buy->head_order = lowest_stop_buy->get_head_order()->get_next_order();
      if (lowest_stop_buy->head_order == nullptr) {
        remove_stop_limit(lowest_stop_buy, buy_or_sell);
      } else { // TODO: Verify this change
        lowest_stop_buy->head_order->set_prev_order(nullptr);
      }
    }
  } else {
    while (highest_stop_sell != nullptr &&
           (highest_buy == nullptr ||
            highest_stop_sell->get_price() <= highest_buy->get_price())) {
      std::cout << std::left 
                << "ID " << std::setw(8) << highest_stop_sell->get_head_order()->get_id()
                << std::setw(5) << "Sell" << std::setw(37)
                << "[STOP ORDER]" << " as Limit Order @ " << highest_stop_sell->get_price() << "\n";
      add_limit_order((highest_stop_sell->get_head_order()));
      highest_stop_sell->head_order =
          highest_stop_sell->get_head_order()->get_next_order();
      if (highest_stop_sell->head_order == nullptr) {
        remove_stop_limit(highest_stop_sell, buy_or_sell);
      } else { // TODO: Verify this change
        highest_stop_sell->head_order->set_prev_order(nullptr);
      }
    }
  }
}

bool OrderBook::add_stop_order_as_market_or_limit_order(Limit *opposite_edge_limit,
                                                 Order* order) {
  if (order->get_buy_or_sell() &&
      (opposite_edge_limit == nullptr ||
       (opposite_edge_limit != nullptr &&
        order->get_stop_price() >= opposite_edge_limit->get_price()))) {
    // TODO: If stop_price is greater than opposite_edge_limit, shouldn't we check
    // for limit_price; if it is 0 shouldn't we try to execute the order
    // immeidately?
    add_limit_order(order);
    return true;
  } else if (!order->get_buy_or_sell() &&
             (opposite_edge_limit == nullptr ||
              (opposite_edge_limit != nullptr &&
               order->get_stop_price() <= opposite_edge_limit->get_price()))) {
    // TODO: If stop_price is less than opposite_edge_limit, shouldn't we check for
    // limit_price; if it is 0 shouldn't we try to execute the order immeidately?
    add_limit_order(order);
    return true;
  }

  return false;
}

void OrderBook::add_limit_order(Order* order) {
  int limit_price = order->get_limit_price();
  std::unordered_map<int, Limit *> *limit_map =
      order->get_buy_or_sell() ? buy_limit_map : sell_limit_map;

  LimitType limitType = order->get_buy_or_sell() ? LimitType::LimitBuy : LimitType::LimitSell;

  if (limit_map->find(limit_price) == limit_map->end()) {
    // TODO: If limit_price is potentially zero, consider having 0 be the min/max
    // on the tree depending on the side (buy/sell)
    Limit* limit = new Limit(order->get_limit_price(), limitType, order);
    limit_map->try_emplace(order->get_limit_price(), limit);
    AvlTree<Limit>*& tree = order->get_buy_or_sell() ? buy_tree : sell_tree;
    if (tree == nullptr) {
      tree = new AvlTree<Limit>(limit);
    } else {
       tree->insert_node(limit);
    }
    update_book_edge_on_insert(limit, limitType);
    return;
  }

  Limit *limit = (*limit_map)[limit_price];
  limit->add_order(order);
}

void OrderBook::update_book_edge_on_insert(Limit *new_limit, const LimitType &type) {
  if (type == LimitType::LimitBuy &&
      (highest_buy == nullptr ||
       (new_limit->get_price() > highest_buy->get_price()))) {
    highest_buy = new_limit;
    return;
  }

  if (type == LimitType::LimitSell &&
      (lowest_sell == nullptr ||
       (new_limit->get_price() < lowest_sell->get_price()))) {
    lowest_sell = new_limit;
    return;
  }

  if (type == LimitType::StopBuy &&
      (lowest_stop_buy == nullptr ||
       (new_limit->get_price() < lowest_stop_buy->get_price()))) {
    lowest_stop_buy = new_limit;
    return;
  }
  if (type == LimitType::StopSell &&
      (highest_stop_sell == nullptr ||
       (new_limit->get_price() > highest_stop_sell->get_price()))) {
    highest_stop_sell = new_limit;
    return;
  }
}

void OrderBook::add_stop_order(Order* order) {
  int stop_price = order->get_stop_price();

  std::unordered_map<int, Limit *> *stop_map =
      order->get_buy_or_sell() ? stop_buy_map : stop_sell_map;

  LimitType limitType = order->get_buy_or_sell() ? LimitType::StopBuy : LimitType::StopSell;

  if (stop_map->find(stop_price) == stop_map->end()) {
    Limit* limit = new Limit(stop_price, limitType, order);
    stop_map->try_emplace(stop_price, limit);
    AvlTree<Limit>*& tree = order->get_buy_or_sell() ? stop_buy_tree : stop_sell_tree;
    if (tree == nullptr) {
      tree = new AvlTree<Limit>(limit);
    } else {
      tree->insert_node(limit);
    }
    update_book_edge_on_insert(limit, limitType);
    return;
  }

  Limit *stop_limit = (*stop_map)[stop_price];
  stop_limit->add_order(order);
}

void OrderBook::update_book_edge_on_delete(Limit *limit, const LimitType &type) {
  if (type == LimitType::LimitBuy || type == LimitType::LimitSell) {
    Limit*& edge = type == LimitType::LimitBuy ? highest_buy : lowest_sell;
    if (limit == edge) {
      if (type == LimitType::LimitBuy && limit->left_child != nullptr) {
        highest_buy = limit->left_child;
      } else if (type == LimitType::LimitSell && limit->right_child != nullptr) {
        lowest_sell = limit->right_child;
      } else {
        edge = limit->parent;
      }
    }
  } else {
    Limit*& edge = type == LimitType::StopBuy ? lowest_stop_buy : highest_stop_sell;

    if (limit == edge) {
      if (type == LimitType::StopBuy && limit->right_child != nullptr) {
        lowest_stop_buy = limit->right_child;
      } else if (type == LimitType::StopSell && limit->left_child != nullptr) {
        highest_stop_sell = limit->left_child;
      } else {
        edge = limit->parent;
      }
    }
  }
}

void OrderBook::remove_limit(Limit *limit, bool buy_or_sell) {
  AvlTree<Limit>* tree = buy_or_sell ? buy_tree : sell_tree;
  std::unordered_map<int, Limit *> *limit_map = buy_or_sell ? buy_limit_map : sell_limit_map;

  limit_map->erase(limit->get_price());
  update_book_edge_on_delete(limit, buy_or_sell ? LimitType::LimitBuy : LimitType::LimitSell);
  tree->delete_node(limit);
}

void OrderBook::remove_stop_limit(Limit *limit, bool buy_or_sell) {
  AvlTree<Limit>* tree = buy_or_sell ? stop_buy_tree : stop_sell_tree;
  std::unordered_map<int, Limit *> *stop_limit_map = buy_or_sell ? stop_buy_map : stop_sell_map;

  stop_limit_map->erase(limit->get_price());
  update_book_edge_on_delete(limit, buy_or_sell ? LimitType::StopBuy : LimitType::StopSell);
  tree->delete_node(limit);
}
