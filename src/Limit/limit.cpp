#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../Order/order.h"
#include "../OrderBook/order_book.h"
#include "limit.h"

Limit::Limit(OrderBook *orderBook, int limitPrice, int stopPrice, Limit *parent)
    : orderBook(orderBook), limitPrice(limitPrice), stopPrice(stopPrice),
      parent(parent) {}

Limit::Limit(OrderBook *orderBook, int limitPrice, Limit *parent)
    : orderBook(orderBook), limitPrice(limitPrice), parent(parent) {}

int Limit::getLimitPrice() const { return limitPrice; }

int Limit::getStopPrice() const { return stopPrice; }

void Limit::addOrder(Order *order) {
  if (order == nullptr) {
    throw std::invalid_argument("Order can not be a nullptr");
  }

  if (headOrder == nullptr) {
    headOrder = tailOrder = order;
  } else {
    tailOrder->nextOrder = order;
    order->prevOrder = tailOrder;
    tailOrder = order;
  }

  volume += order->getShares();
}

int Limit::getVolume() const { return volume; }

Order *Limit::getHeadOrder() const { return headOrder; }
Order *Limit::getTailOrder() const { return tailOrder; }

void Limit::execute(Order *oppositeHeadOrder, Order &order) {
  if (oppositeHeadOrder->getShares() <= order.getShares()) {
    int filledShares = oppositeHeadOrder->getShares();

    this->volume -= oppositeHeadOrder->getShares();
    order.setShares(order.getShares() - oppositeHeadOrder->getShares());

    std::cout << std::left << (order.getBuyOrSell() ? "Buy" : "Sell") << " "
              << (order.getShares() == 0 ? "[FILL]" : "[PARTIAL FILL]")
              << std::setw(29) << " for " << filledShares << " @ "
              << oppositeHeadOrder->getLimitPrice() << "\n";

    Order *tempOrder = oppositeHeadOrder;
    oppositeHeadOrder = headOrder->nextOrder;
    if (oppositeHeadOrder != nullptr) {
      oppositeHeadOrder->prevOrder = nullptr;

      delete tempOrder;
    } else {
      orderBook->deleteLimitFromAVLTree(this, !order.getBuyOrSell());
      delete oppositeHeadOrder;
      delete tempOrder;
    }
    return;
  }
  int filledShares = order.getShares();
  this->volume -= order.getShares();
  oppositeHeadOrder->setShares(headOrder->getShares() - order.getShares());

  std::cout << std::left << (order.getBuyOrSell() ? "Buy" : "Sell") << " [FILL]"
            << std::setw(29) << " for " << filledShares << " @ "
            << oppositeHeadOrder->getLimitPrice() << "\n";
  return;
}
