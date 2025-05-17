#include "order.h"

Order::Order(int id, int shares, int buyOrSell, int limitPrice, int stopPrice)
    : id(id), shares(shares), buyOrSell(buyOrSell), limitPrice(limitPrice),
      stopPrice(stopPrice) {}

int Order::getId() const { return id; }

int Order::getShares() const { return shares; }
int Order::getFilledShares() const { return filledShares; }

void Order::setShares(int _shares) { shares = _shares; }
void Order::setFilledShares(int _filledShares) { filledShares = _filledShares; }

int Order::getLimitPrice() const { return limitPrice; }
int Order::getStopPrice() const { return stopPrice; }

int Order::getBuyOrSell() const { return buyOrSell; }

Order *Order::getNextOrder() { return nextOrder; }
Order *Order::getPrevOrder() { return prevOrder; }

void Order::setPrevOrder(Order *order) { prevOrder = order; }
void Order::setNextOrder(Order *order) { nextOrder = order; }
