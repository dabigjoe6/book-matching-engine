#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../Order/order.h"
#include "../OrderBook/order_book.h"
#include "limit.h"

Limit::Limit(int _price, LimitType _type): Node<Limit, int>(_price), price(_price), type(_type) {};


void Limit::add_order(Order *order) {
  if (order == nullptr) {
    throw std::invalid_argument("Order can not be a nullptr");
  }

  if (head_order == nullptr) {
    head_order = tail_order = order;
  } else {
    tail_order->next_order = order;
    order->prev_order = tail_order;
    tail_order = order;
  }

  volume += order->get_shares();
}


int Limit::get_price() const { return value; }
int Limit::get_shares() const { return shares; }
int Limit::get_volume() const { return volume; }

Order *Limit::get_head_order() const { return head_order; }
Order *Limit::get_tail_order() const { return tail_order; }
