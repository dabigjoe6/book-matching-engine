#include <algorithm>
#include <iomanip>
#include <iostream>

#include "order_book.h"

#include "../Limit/limit.h"
#include "../Order/order.h"

OrderBook::OrderBook() {}

Limit *OrderBook::getHighestBuy() { return highestBuy; }
Limit *OrderBook::getLowestSell() { return lowestSell; }

Limit *OrderBook::getLowestStopBuy() { return lowestStopBuy; }
Limit *OrderBook::getHighestStopSell() { return highestStopSell; }

void OrderBook::addOrder(Order &newOrder) {
  // market order
  if (newOrder.getLimitPrice() == 0 && newOrder.getStopPrice() == 0) {
    std::cout << std::left << std::setw(5)
              << (newOrder.getBuyOrSell() ? "Buy" : "Sell") << std::setw(29)
              << "[MARKET ORDER SUBMITTED]" << "for " << newOrder.getShares()
              << "\n";
    addMarketOrder(newOrder);
  }

  // limit order
  if (newOrder.getLimitPrice() > 0 && newOrder.getStopPrice() == 0) {
    std::cout << std::left << std::setw(5)
              << (newOrder.getBuyOrSell() ? "Buy" : "Sell") << std::setw(29)
              << "[LIMIT ORDER SUBMITTED]" << " for " << newOrder.getShares()
              << " @ Limit Price: " << newOrder.getLimitPrice() << "\n";
    addLimitOrder(newOrder);
  }

  // stop order or stop limit order
  if (newOrder.getStopPrice() > 0) {
    if (newOrder.getStopPrice() > 0 && newOrder.getLimitPrice() == 0) {
      std::cout << std::left << std::setw(5)
                << (newOrder.getBuyOrSell() ? "Buy" : "Sell") << std::setw(29)
                << "[STOP ORDER SUBMITTED]" << " for " << newOrder.getShares()
                << " @ Limit Price: " << newOrder.getLimitPrice()
                << " @ Stop Price:" << newOrder.getStopPrice() << "\n";
    } else {
      std::cout << std::left << std::setw(5)
                << (newOrder.getBuyOrSell() ? "Buy" : "Sell") << std::setw(29)
                << "[STOP LIMIT ORDER SUBMITTED]" << " for "
                << newOrder.getShares()
                << " @ Limit Price: " << newOrder.getLimitPrice()
                << " @ Stop Price: " << newOrder.getStopPrice() << "\n";
    }
    addStopOrder(newOrder);
  }
}

int OrderBook::marketOrderHelper(Limit *edge, Order &order) {
  // TODO: What determines the price of the edge when limit price is 0
  // Do we just match all possible shares automatically? - What does the real
  // world scenario look like?
  while (
      edge != nullptr &&
      ((order.getLimitPrice() == 0 ||
        ((order.getBuyOrSell() && order.getLimitPrice() >= edge->getPrice()) ||
         (!order.getBuyOrSell() &&
          order.getLimitPrice() <= edge->getPrice()))) &&
       (edge->getHeadOrder() != nullptr && order.getShares() != 0))) {
    edge->execute(edge->getHeadOrder(), order);
  }

  return order.getShares();
}

void OrderBook::executeStopOrders(int buyOrSell) {
  if (buyOrSell) {
    while (lowestStopBuy != nullptr &&
           (lowestSell == nullptr ||
            lowestStopBuy->getPrice() >= lowestSell->getPrice())) {
      std::cout << std::left << std::setw(5) << "Buy" << std::setw(29)
                << "[STOP ORDER]" << " @ " << lowestStopBuy->getPrice()
                << " as Limit Order @ " << lowestStopBuy->getPrice() << "\n";
      addLimitOrder(*(lowestStopBuy->getHeadOrder()));
      lowestStopBuy->headOrder = lowestStopBuy->getHeadOrder()->getNextOrder();
      if (lowestStopBuy->headOrder == nullptr) {
        deleteStopLimitFromAVLTree(lowestStopBuy, buyOrSell);
      }

      lowestStopBuy->headOrder->setPrevOrder(nullptr);
    }
  } else {
    while (highestStopSell != nullptr &&
           (highestBuy == nullptr ||
            highestStopSell->getPrice() <= highestBuy->getPrice())) {
      std::cout << std::left << std::setw(5) << "Sell" << std::setw(29)
                << "[STOP ORDER]" << "for" << lowestStopBuy->getPrice()
                << " as Limit Order @ " << lowestStopBuy->getPrice() << "\n";
      addLimitOrder(*(highestStopSell->getHeadOrder()));
      highestStopSell->headOrder =
          highestStopSell->getHeadOrder()->getNextOrder();
      if (highestStopSell->headOrder == nullptr) {
        deleteStopLimitFromAVLTree(highestStopSell, buyOrSell);
      }
    }
  }
}

void OrderBook::addMarketOrder(Order &order) {
  Limit *oppositeEdgeLimit =
      order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

  // If we the order is partially filled or there's no existing order to match
  // against, we forget the order
  marketOrderHelper(oppositeEdgeLimit, order);

  executeStopOrders(order.getBuyOrSell());
}

void OrderBook::addLimitOrder(Order &order) {
  Limit *oppositeEdgeLimit =
      order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

  int shares = marketOrderHelper(oppositeEdgeLimit, order);

  if (shares != 0) {
    addLimitOrderToLimitQueue(order);
  }

  executeStopOrders(order.getBuyOrSell());
}

void OrderBook::addLimitOrderToLimitQueue(Order &order) {
  int limitPrice = order.getLimitPrice();
  std::unordered_map<int, Limit *> *limitMap =
      order.getBuyOrSell() ? buyLimitMap : sellLimitMap;

  if (limitMap->find(limitPrice) == limitMap->end()) {
    // TODO: If limitPrice is potentially zero, consider having 0 be the min/max
    // on the tree depending on the side (buy/sell)
    insertLimitIntoAVLTree(limitPrice, order.getBuyOrSell());
  }

  Limit *limit = (*limitMap)[limitPrice];
  limit->addOrder(&order);
}

void OrderBook::insertLimitIntoAVLTree(const int price, const int buyOrSell) {
  Limit *tree = buyOrSell ? buyTree : sellTree;
  Limit *edge = buyOrSell ? highestBuy : lowestSell;
  std::unordered_map<int, Limit *> *limitMap =
      buyOrSell ? buyLimitMap : sellLimitMap;

  LimitType type = buyOrSell ? LimitType::LimitBuy : LimitType::LimitSell;

  if (tree == nullptr) {
    Limit *newLimit = new Limit(this, price, type, nullptr);
    tree = newLimit;
    edge = tree;

    if (buyOrSell) {
      buyTree = tree;
    } else {
      sellTree = tree;
    }

    limitMap->try_emplace(price, newLimit);

    return;
  }

  _insert(tree, price, type, limitMap, nullptr);
}

void OrderBook::updateBookEdgeOnInsert(Limit *newLimit, const LimitType &type) {
  if (type == LimitType::LimitSell || type == LimitType::LimitBuy) {
    if (type == LimitType::LimitBuy) {
      if (highestBuy != nullptr &&
          newLimit->getPrice() > highestBuy->getPrice()) {
        highestBuy = newLimit;
      } else if (highestBuy == nullptr) {
        highestBuy = newLimit;
      }
    } else {
      if (lowestSell != nullptr &&
          newLimit->getPrice() < lowestSell->getPrice()) {
        lowestSell = newLimit;
      } else if (lowestSell == nullptr) {
        lowestSell = newLimit;
      }
    }
  } else {
    if (type == LimitType::StopBuy) {
      if (lowestStopBuy != nullptr &&
          newLimit->getPrice() < lowestStopBuy->getPrice()) {
        lowestStopBuy = newLimit;
      } else if (lowestStopBuy == nullptr) {
        lowestStopBuy = newLimit;
      }
    } else {
      if (highestStopSell != nullptr &&
          newLimit->getPrice() > highestStopSell->getPrice()) {
        highestStopSell = newLimit;
      } else if (highestStopSell == nullptr) {
        highestStopSell = newLimit;
      }
    }
  }
}

void OrderBook::addStopOrder(Order &order) {
  Limit *oppositeEdgeLimit =
      order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

  if (!addStopOrderAsMarketOrLimitOrder(oppositeEdgeLimit, order)) {
    addStopOrderToStopQueue(order);
  }
}

bool OrderBook::addStopOrderAsMarketOrLimitOrder(Limit *oppositeEdgeLimit,
                                                 Order &order) {
  if (order.getBuyOrSell() &&
      (oppositeEdgeLimit == nullptr ||
       (oppositeEdgeLimit != nullptr &&
        order.getStopPrice() >= oppositeEdgeLimit->getPrice()))) {
    // TODO: If stopPrice is greater than oppositeEdgeLimit, shouldn't we check
    // for limitPrice; if it is 0 shouldn't we try to execute the order
    // immeidately?
    addLimitOrder(order);
    return true;
  } else if (!order.getBuyOrSell() &&
             (oppositeEdgeLimit == nullptr ||
              (oppositeEdgeLimit != nullptr &&
               order.getStopPrice() <= oppositeEdgeLimit->getPrice()))) {
    // TODO: If stopPrice is less than oppositeEdgeLimit, shouldn't we check for
    // limitPrice; if it is 0 shouldn't we try to execute the order immeidately?
    addLimitOrder(order);
    return true;
  }

  return false;
}

void OrderBook::addStopOrderToStopQueue(Order &order) {
  int stopPrice = order.getStopPrice();
  int limitPrice = order.getLimitPrice();

  std::unordered_map<int, Limit *> *stopMap =
      order.getBuyOrSell() ? stopBuyMap : stopSellMap;

  if (stopMap->find(stopPrice) == stopMap->end()) {
    insertStopLimitIntoAVLTree(stopPrice, order.getBuyOrSell());
  }

  Limit *stopLimit = (*stopMap)[stopPrice];
  stopLimit->addOrder(&order);
}

void OrderBook::insertStopLimitIntoAVLTree(const int price,
                                           const int buyOrSell) {
  Limit *tree = buyOrSell ? stopBuyTree : stopSellTree;
  Limit *edge = buyOrSell ? lowestStopBuy : highestStopSell;

  std::unordered_map<int, Limit *> *stopMap =
      buyOrSell ? stopBuyMap : stopSellMap;

  LimitType type = buyOrSell ? LimitType::StopBuy : LimitType::StopSell;

  if (tree == nullptr) {
    Limit *newStopLimit = new Limit(this, price, type);
    tree = newStopLimit;
    edge = tree;

    stopMap->try_emplace(price, newStopLimit);

    return;
  }

  _insert(tree, price, type, stopMap, nullptr);
}

Limit *OrderBook::_insert(Limit *root, const int &price, const LimitType &type,
                          std::unordered_map<int, Limit *> *limitMap,
                          Limit *parent) {
  if (root == nullptr) {
    Limit *newLimit = new Limit(this, price, type, parent);
    limitMap->try_emplace(price, newLimit);
    updateBookEdgeOnInsert(newLimit, type);
    return newLimit;
  }

  if (price < root->price) {
    root->leftLimit = _insert(root->leftLimit, price, type, limitMap, root);
  } else if (price > root->price) {
    root->rightLimit = _insert(root->rightLimit, price, type, limitMap, root);
  } else {
    return root; // no duplicate limitPrice
  }

  updateHeight(root);
  int balance = getBalance(root);

  // Left Left case
  if (balance > 1 && price < root->leftLimit->price) {
    return rotateRight(root);
  }

  // Right Right case
  if (balance < -1 && price > root->rightLimit->price) {
    return rotateLeft(root);
  }

  // Left Right case
  if (balance > 1 && price > root->leftLimit->price) {
    root->leftLimit = rotateLeft(root->leftLimit);
    return rotateRight(root);
  }

  // Right Left case
  if (balance < -1 && price < root->rightLimit->price) {
    root->rightLimit = rotateRight(root->rightLimit);
    return rotateLeft(root);
  }

  return root;
}

void OrderBook::deleteLimitFromAVLTree(Limit *limit, int buyOrSell) {
  Limit *tree = buyOrSell ? buyTree : sellTree;

  std::unordered_map<int, Limit *> *limitMap =
      buyOrSell ? buyLimitMap : sellLimitMap;

  LimitType type = buyOrSell ? LimitType::LimitBuy : LimitType::LimitSell;

  updateBookEdgeOnDelete(limit, type);
  _delete(tree, limit->getPrice());
  limitMap->erase(limit->getPrice());
}

void OrderBook::updateBookEdgeOnDelete(Limit *limit, const LimitType &type) {
  Limit *edge;

  if (type == LimitType::LimitBuy || type == LimitType::LimitSell) {
    edge = type == LimitType::LimitBuy ? highestBuy : lowestSell;
    if (limit == edge) {
      if (type == LimitType::LimitBuy && limit->leftLimit != nullptr) {
        highestBuy = limit->leftLimit;
      } else if (type == LimitType::LimitSell && limit->rightLimit != nullptr) {
        lowestSell = limit->rightLimit;
      } else {
        edge = limit->parent;
      }
    }
  } else {
    edge = type == LimitType::StopBuy ? lowestStopBuy : highestStopSell;

    if (limit == edge) {
      if (type == LimitType::StopBuy && limit->rightLimit != nullptr) {
        lowestStopBuy = limit->rightLimit;
      } else if (type == LimitType::StopSell && limit->leftLimit != nullptr) {
        highestStopSell = limit->leftLimit;
      } else {
        edge = limit->parent;
      }
    }
  }
}

Limit *OrderBook::_delete(Limit *root, const int price) {
  if (root == nullptr) {
    return root;
  }

  if (price < root->price) {
    root->leftLimit = _delete(root->leftLimit, price);
  } else if (price > root->price) {
    root->rightLimit = _delete(root->rightLimit, price);
  } else {
    if (root->leftLimit == nullptr) {
      Limit *temp = root->rightLimit;
      delete root;
      return temp;
    } else if (root->rightLimit == nullptr) {
      Limit *temp = root->leftLimit;
      delete root;
      return temp;
    }

    Limit *temp = getMinValueNode(root->rightLimit);
    root->price = temp->price;
    root->rightLimit = _delete(root->rightLimit, temp->price);
  }

  if (root == nullptr)
    return root;

  updateHeight(root);
  int balance = getBalance(root);

  // Left Left case
  if (balance > 1 && getBalance(root->leftLimit) >= 0) {
    return rotateRight(root);
  }

  // Left Right case
  if (balance > 1 && getBalance(root->leftLimit) < 0) {
    root->leftLimit = rotateLeft(root->leftLimit);
    return rotateRight(root);
  }

  // Right Right case
  if (balance < -1 && getBalance(root->rightLimit) <= 0) {
    return rotateLeft(root);
  }

  // Right Left case
  if (balance < -1 && getBalance(root->rightLimit) > 0) {
    root->rightLimit = rotateRight(root);
    return rotateLeft(root);
  }

  return root;
}

Limit *OrderBook::_stopDelete(Limit *root, const int price) {
  if (root == nullptr) {
    return root;
  }

  if (price < root->price) {
    root->leftLimit = _delete(root->leftLimit, price);
  } else if (price > root->price) {
    root->rightLimit = _delete(root->rightLimit, price);
  } else {
    if (root->leftLimit == nullptr) {
      Limit *temp = root->rightLimit;
      delete root;
      return temp;
    } else if (root->rightLimit == nullptr) {
      Limit *temp = root->leftLimit;
      delete root;
      return temp;
    }

    Limit *temp = getMinValueNode(root->rightLimit);
    root->price = temp->price;
    root->rightLimit = _delete(root->rightLimit, temp->price);
  }

  if (root == nullptr)
    return root;

  updateHeight(root);
  int balance = getBalance(root);

  // Left Left case
  if (balance > 1 && getBalance(root->leftLimit) >= 0) {
    return rotateRight(root);
  }

  // Left Right case
  if (balance > 1 && getBalance(root->leftLimit) < 0) {
    root->leftLimit = rotateLeft(root->leftLimit);
    return rotateRight(root);
  }

  // Right Right case
  if (balance < -1 && getBalance(root->rightLimit) <= 0) {
    return rotateLeft(root);
  }

  // Right Left case
  if (balance < -1 && getBalance(root->rightLimit) > 0) {
    root->rightLimit = rotateRight(root);
    return rotateLeft(root);
  }

  return root;
}

void OrderBook::deleteStopLimitFromAVLTree(Limit *limit, int buyOrSell) {
  Limit *tree = buyOrSell ? stopBuyTree : stopSellTree;

  std::unordered_map<int, Limit *> *stopLimitMap =
      buyOrSell ? stopBuyMap : stopSellMap;

  LimitType type = buyOrSell ? LimitType::StopBuy : LimitType::StopSell;

  updateBookEdgeOnDelete(limit, type);
  _stopDelete(tree, limit->getPrice());
  stopLimitMap->erase(limit->getPrice());
}

Limit *OrderBook::getMinValueNode(Limit *node) {
  Limit *current = node;
  while (current->leftLimit != nullptr) {
    current = current->leftLimit;
  }
  return current;
}

Limit *OrderBook::rotateLeft(Limit *node) {
  Limit *newRoot = node->rightLimit;
  node->rightLimit = newRoot->leftLimit;
  if (newRoot->leftLimit != nullptr) {
    newRoot->leftLimit->parent = node;
  }

  newRoot->leftLimit = node;
  newRoot->parent = node->parent;
  node->parent = newRoot;
  updateHeight(node);
  updateHeight(newRoot);

  return newRoot;
}

Limit *OrderBook::rotateRight(Limit *node) {
  Limit *newRoot = node->leftLimit;
  node->leftLimit = newRoot->rightLimit;
  if (newRoot->rightLimit != nullptr) {
    newRoot->rightLimit->parent = node;
  }

  newRoot->rightLimit = node;
  newRoot->parent = node->parent;
  node->parent = newRoot;
  updateHeight(node);
  updateHeight(newRoot);

  return newRoot;
}

void OrderBook::updateHeight(Limit *root) {
  root->height =
      1 + std::max(getHeight(root->rightLimit), getHeight(root->leftLimit));
}

int OrderBook::getHeight(const Limit *limit) {
  if (limit == nullptr)
    return -1;

  return limit->height;
}

int OrderBook::getBalance(const Limit *node) {
  if (node == nullptr)
    return 0;

  return getHeight(node->leftLimit) - getHeight(node->rightLimit);
}
