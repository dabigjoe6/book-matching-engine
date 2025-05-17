#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../Order/order.h"
#include "../OrderBook/order_book.h"
#include "limit.h"

Limit::Limit(OrderBook *orderBook, int price, LimitType type, Limit *parent)
    : orderBook(orderBook), price(price), type(type), parent(parent) {}

int Limit::getPrice() const { return price; }

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

void Limit::execute(Order *oppositeHeadOrder, Order *order) {
  if (oppositeHeadOrder->getShares() <= order->getShares()) {
    int filledShares = oppositeHeadOrder->getShares();
    
    // We will always deduct from volume and shares because we are 
    // simulating filled order regardless of buy/sell side
    this->volume -= oppositeHeadOrder->getShares();
    
    order->setShares(order->getShares() - oppositeHeadOrder->getShares());
    order->setFilledShares(filledShares + order->getFilledShares());

    std::cout << std::left << std::setw(5)
              << (order->getBuyOrSell() ? "Buy" : "Sell") << std::setw(29)
              << (order->getShares() == 0 ? "[FILL]" : "[PARTIAL FILL]")
              << " for " << filledShares << " @ "
              << "Limit Price: " << oppositeHeadOrder->getLimitPrice() << "\n";

    headOrder = headOrder->nextOrder;
    if (headOrder != nullptr) {
      headOrder->prevOrder = nullptr;
    } else {
      orderBook->deleteLimitFromAVLTree(this, !order->getBuyOrSell());
      
      // TODO: Need to record somewhere completelly filled orders before deleting the order instance
      delete headOrder;
      headOrder = nullptr;
    }
    return;
  }

  int filledShares = order->getShares();
  
  this->volume -= order->getShares();
  
  oppositeHeadOrder->setShares(headOrder->getShares() - order->getShares());
  oppositeHeadOrder->setFilledShares(headOrder->getFilledShares() + order->getShares());

  // TODO: Need to record somewhere completelly filled orders before deleting the order instance
  delete order;

  std::cout << std::left << std::setw(5)
            << (order->getBuyOrSell() ? "Buy" : "Sell") << std::setw(29)
            << "[FILL]" << " for " << filledShares << " @ "
            << "Limit Price: " << oppositeHeadOrder->getLimitPrice() << "\n";

  return;
}
