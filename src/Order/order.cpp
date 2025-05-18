#include "order.h"

Order::Order(int id, int shares, int buy_or_sell, int limit_price, int stop_price)
    : id(id), shares(shares), buy_or_sell(buy_or_sell), limit_price(limit_price),
      stop_price(stop_price) {}

int Order::get_id() const { return id; }

int Order::get_shares() const { return shares; }
int Order::get_filled_shares() const { return filled_shares; }

void Order::set_shares(int _shares) { shares = _shares; }
void Order::set_filled_shares(int _filled_shares) { filled_shares = _filled_shares; }

int Order::get_limit_price() const { return limit_price; }
int Order::get_stop_price() const { return stop_price; }

int Order::get_buy_or_sell() const { return buy_or_sell; }

Order *Order::get_next_order() { return next_order; }
Order *Order::get_prev_order() { return prev_order; }

void Order::set_prev_order(Order *order) { prev_order = order; }
void Order::set_next_order(Order *order) { next_order = order; }
