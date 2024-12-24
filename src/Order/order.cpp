#include "order.h"

Order::Order(int shares, int buyOrSell, int limitPrice, int stopPrice)
    : shares(shares), buyOrSell(buyOrSell), limitPrice(limitPrice),
      stopPrice(stopPrice) {}

int Order::getShares() const { return shares; }
void Order::setShares(int _shares) { shares = _shares; }

int Order::getLimitPrice() const { return limitPrice; }
int Order::getStopPrice() const { return stopPrice; }

int Order::getBuyOrSell() const { return buyOrSell; }

Order *Order::getNextOrder() { return nextOrder; }
Order *Order::getPrevOrder() { return prevOrder; }

void Order::setPrevOrder(Order *order) { prevOrder = order; }
void Order::setNextOrder(Order *order) { nextOrder = order; }
