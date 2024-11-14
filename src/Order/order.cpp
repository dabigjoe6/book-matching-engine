#include "order.h"

Order::Order(int shares, int buyOrSell, int limitPrice, int stopPrice): shares(shares), buyOrSell(buyOrSell), limitPrice(limitPrice), stopPrice(stopPrice) {}

int Order::getShares() { return shares; }

void Order::setShares(int _shares) { shares = _shares; }

int Order::getLimitPrice() { return limitPrice; }

int Order::getStopPrice() { return stopPrice; }

int Order::getBuyOrSell() { return buyOrSell; }
