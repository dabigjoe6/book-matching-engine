#include <algorithm>

#include "order_book.h"

#include "../Limit/limit.h"
#include "../Order/order.h"

OrderBook::OrderBook() {}

void OrderBook::addOrder(Order &newOrder) {
  // market order
  if (newOrder.getLimitPrice() == 0 && newOrder.getStopPrice() == 0) {
    addMarketOrder(newOrder);
  }

  // limit order
  if (newOrder.getLimitPrice() > 0 && newOrder.getStopPrice() == 0) {
    addLimitOrder(newOrder);
  }

  // stop order or stop limit order
  if (newOrder.getStopPrice() > 0) {
    addStopOrder(newOrder);
  }
}

int OrderBook::marketOrderHelper(Limit *edge, Order &order) {
  while (((order.getLimitPrice() == 0 ||
           ((order.getBuyOrSell() &&
             order.getLimitPrice() >= edge->getLimitPrice()) ||
            (!order.getBuyOrSell() &&
             order.getLimitPrice() <= edge->getLimitPrice()))) &&
          (edge != nullptr && edge->getHeadOrder() != nullptr &&
           order.getShares() != 0))) {
    edge->execute(edge->getHeadOrder(), order);
  }

  return order.getShares();
}

void OrderBook::executeStopOrders(int buyOrSell) {
  if (buyOrSell) {
    while (lowestStopBuy != nullptr &&
           (lowestSell == nullptr ||
            lowestStopBuy->getLimitPrice() >= lowestSell->getLimitPrice())) {
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
            highestStopSell->getLimitPrice() <= highestBuy->getLimitPrice())) {
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
  Limit *edgeLimit =
      order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

  // If we the order is partially filled or there's no existing order to match
  // against, we forget the order
  marketOrderHelper(edgeLimit, order);

  executeStopOrders(order.getBuyOrSell());
}

bool OrderBook::addStopOrderAsMarketOrLimitOrder(Limit *edgeLimit,
                                                 Order &order) {
  if (order.getBuyOrSell() &&
      (edgeLimit == nullptr ||
       (edgeLimit != nullptr &&
        order.getStopPrice() >= edgeLimit->getLimitPrice()))) {
    addLimitOrder(order);
    return true;
  } else if (!order.getBuyOrSell() &&
             (edgeLimit == nullptr ||
              (edgeLimit != nullptr &&
               order.getStopPrice() <= edgeLimit->getLimitPrice()))) {
    addLimitOrder(order);
    return true;
  }

  return false;
}

void OrderBook::addLimitOrder(Order &order) {
  Limit *edgeLimit =
      order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

  int shares = marketOrderHelper(edgeLimit, order);

  if (shares != 0) {
    queueOrderInLimit(order);
  }

  executeStopOrders(order.getBuyOrSell());
}

void OrderBook::queueOrderInLimit(Order &order) {
  int limitPrice = order.getLimitPrice();
  std::unordered_map<int, Limit *>* limitMap =
      order.getBuyOrSell() ? buyLimitMap : sellLimitMap;

  if (limitMap->find(limitPrice) == limitMap->end()) {
    insertLimitIntoAVLTree(limitPrice, order.getBuyOrSell());
  }

  Limit *limit = (*limitMap)[limitPrice];
  limit->addOrder(&order);
}

void OrderBook::addStopOrder(Order &order) {
  Limit *edgeLimit =
      order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

  if (!addStopOrderAsMarketOrLimitOrder(edgeLimit, order)) {
    queueStopOrderInLimit(order);
  }
}

void OrderBook::queueStopOrderInLimit(Order &order) {
  int stopPrice = order.getStopPrice();
  int limitPrice = order.getLimitPrice();

  std::unordered_map<int, Limit *>* stopMap =
      order.getBuyOrSell() ? stopBuyMap : stopSellMap;

  if (stopMap->find(stopPrice) == stopMap->end()) {
    insertStopLimitIntoAVLTree(stopPrice, limitPrice, order.getBuyOrSell());
  }

  Limit *stopLimit = (*stopMap)[stopPrice];
  stopLimit->addOrder(&order);
}

void OrderBook::insertLimitIntoAVLTree(const int limitPrice, const int buyOrSell) {
  Limit *tree = buyOrSell ? buyTree : sellTree;
  Limit *edge = buyOrSell ? highestBuy : lowestSell;
  std::unordered_map<int, Limit *>* limitMap = buyOrSell ? buyLimitMap : sellLimitMap;

  if (tree == nullptr) {
    Limit *newLimit = new Limit(this, limitPrice);
    tree = newLimit;
    edge = tree;

    limitMap->try_emplace(limitPrice, newLimit);

    return;
  }

  Limit *newLimit = _insert(tree, limitPrice);
  limitMap->try_emplace(limitPrice, newLimit);
  updateBookEdgeOnInsert(newLimit, buyOrSell);
}

void OrderBook::updateBookEdgeOnInsert(Limit *newLimit, const int buyOrSell) {
  if (buyOrSell) {
    if (highestBuy != nullptr &&
        newLimit->getLimitPrice() > highestBuy->getLimitPrice()) {
      highestBuy = newLimit;
    } else if (highestBuy == nullptr) {
      highestBuy = newLimit;
    }
  } else {
    if (lowestSell != nullptr &&
        newLimit->getLimitPrice() < lowestSell->getLimitPrice()) {
      lowestSell = newLimit;
    } else if (lowestSell == nullptr) {
      lowestSell = newLimit;
    }
  }
}

Limit *OrderBook::getHighestBuy() { return highestBuy; }
Limit *OrderBook::getLowestSell() { return lowestSell; }

Limit *OrderBook::getLowestStopBuy() { return lowestStopBuy; }
Limit *OrderBook::getHighestStopSell() { return highestStopSell; }

void OrderBook::insertStopLimitIntoAVLTree(const int stopPrice, const int limitPrice, const int buyOrSell) {
  Limit *tree = buyOrSell ? stopBuyTree : stopSellTree;
  Limit *edge = buyOrSell ? lowestStopBuy : highestStopSell;

  std::unordered_map<int, Limit *>* stopMap =
      buyOrSell ? stopBuyMap : stopSellMap;

  if (tree == nullptr) {
    Limit *newStopLimit = new Limit(this, limitPrice, stopPrice);
    tree = newStopLimit;
    edge = tree;

    stopMap->try_emplace(stopPrice, newStopLimit);

    return;
  }

  Limit *newStopLimit = _insert(tree, stopPrice, limitPrice);
  stopMap->try_emplace(stopPrice, newStopLimit);
  updateBookStopEdgeOnInsert(newStopLimit, buyOrSell);
}

void OrderBook::updateBookStopEdgeOnInsert(Limit *newStopLimit, const int buyOrSell) {
  Limit *edge = buyOrSell ? lowestStopBuy : highestStopSell;
  if (buyOrSell) {
    if (lowestStopBuy != nullptr &&
        newStopLimit->getStopPrice() < lowestStopBuy->getStopPrice()) {
      lowestStopBuy = newStopLimit;
    } else if (lowestStopBuy == nullptr) {
      lowestStopBuy = newStopLimit;
    }
  } else {
    if (highestStopSell != nullptr &&
        newStopLimit->getStopPrice() > highestStopSell->getStopPrice()) {
      highestStopSell = newStopLimit;
    } else if (highestStopSell == nullptr) {
      highestStopSell = newStopLimit;
    }
  }
}

Limit *OrderBook::_insert(Limit *root, const int limitPrice) {
  if (root == nullptr) {
    return new Limit(this, limitPrice);
  }

  if (limitPrice < root->limitPrice) {
    root->leftLimit = _insert(root->leftLimit, limitPrice);
  } else if (limitPrice > root->limitPrice) {
    root->rightLimit = _insert(root->rightLimit, limitPrice);
  } else {
    return root; // no duplicate limitPrice
  }

  updateHeight(root);
  int balance = getBalance(root);

  // Left Left case
  if (balance > 1 && limitPrice < root->leftLimit->limitPrice) {
    return rotateRight(root);
  }

  // Right Right case
  if (balance < 1 && limitPrice > root->rightLimit->limitPrice) {
    return rotateLeft(root);
  }

  // Left Right case
  if (balance > 1 && limitPrice > root->leftLimit->limitPrice) {
    root->leftLimit = rotateLeft(root->leftLimit);
    return rotateRight(root);
  }

  // Right Left case
  if (balance < 1 && limitPrice < root->rightLimit->limitPrice) {
    root->rightLimit = rotateRight(root->rightLimit);
    return rotateLeft(root);
  }

  return root;
}

Limit *OrderBook::_insert(Limit *root, const int stopPrice, const int limitPrice) {
  if (root == nullptr) {
    return new Limit(this, limitPrice, stopPrice);
  }

  if (stopPrice < root->stopPrice) {
    root->leftLimit = _insert(root->leftLimit, stopPrice, limitPrice);
  } else if (stopPrice > root->stopPrice) {
    root->rightLimit = _insert(root->rightLimit, stopPrice);
  } else {
    return root; // no duplicate limitPrice
  }

  updateHeight(root);
  int balance = getBalance(root);

  // Left Left case
  if (balance > 1 && stopPrice < root->leftLimit->stopPrice) {
    return rotateRight(root);
  }

  // Right Right case
  if (balance < 1 && stopPrice > root->rightLimit->stopPrice) {
    return rotateLeft(root);
  }

  // Left Right case
  if (balance > 1 && stopPrice > root->leftLimit->stopPrice) {
    root->leftLimit = rotateLeft(root->leftLimit);
    return rotateRight(root);
  }

  // Right Left case
  if (balance < 1 && stopPrice < root->rightLimit->stopPrice) {
    root->rightLimit = rotateRight(root->rightLimit);
    return rotateLeft(root);
  }

  return root;
}

void OrderBook::deleteLimitFromAVLTree(Limit *limit, int buyOrSell) {
  Limit *tree = buyOrSell ? buyTree : sellTree;

  std::unordered_map<int, Limit *>* limitMap =
      buyOrSell ? buyLimitMap : sellLimitMap;

  updateBookEdgeOnDelete(limit, buyOrSell);
  _delete(tree, limit->getLimitPrice());
  limitMap->erase(limit->getLimitPrice());
}

void OrderBook::updateBookEdgeOnDelete(Limit *limit, const int buyOrSell) {
  Limit *edge = buyOrSell ? highestBuy : lowestSell;

  if (limit == edge) {
    if (buyOrSell && limit->leftLimit != nullptr) {
      highestBuy = limit->leftLimit;
    } else if (!buyOrSell && limit->rightLimit != nullptr) {
      lowestSell = limit->rightLimit;
    } else {
      edge = limit->parent;
    }
  }
}

void OrderBook::updateBookStopEdgeOnDelete(Limit *limit, const int buyOrSell) {
  Limit *edge = buyOrSell ? lowestStopBuy : highestStopSell;

  if (limit == edge) {
    if (buyOrSell && limit->rightLimit != nullptr) {
      lowestStopBuy = limit->rightLimit;
    } else if (!buyOrSell && limit->leftLimit != nullptr) {
      highestStopSell = limit->leftLimit;
    } else {
      edge = limit->parent;
    }
  }
}

Limit *OrderBook::_delete(Limit *root, const int limitPrice) {
  if (root == nullptr) {
    return root;
  }

  if (limitPrice < root->limitPrice) {
    root->leftLimit = _delete(root->leftLimit, limitPrice);
  } else if (limitPrice > root->limitPrice) {
    root->rightLimit = _delete(root->rightLimit, limitPrice);
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
    root->limitPrice = temp->limitPrice;
    root->rightLimit = _delete(root->rightLimit, temp->limitPrice);
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

Limit *OrderBook::_stopDelete(Limit *root, const int stopPrice) {
  if (root == nullptr) {
    return root;
  }

  if (stopPrice < root->stopPrice) {
    root->leftLimit = _delete(root->leftLimit, stopPrice);
  } else if (stopPrice > root->stopPrice) {
    root->rightLimit = _delete(root->rightLimit, stopPrice);
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
    root->stopPrice = temp->stopPrice;
    root->rightLimit = _delete(root->rightLimit, temp->stopPrice);
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

  std::unordered_map<int, Limit *>* stopLimitMap =
      buyOrSell ? stopBuyMap : stopSellMap;

  updateBookStopEdgeOnDelete(limit, buyOrSell);
  _stopDelete(tree, limit->getStopPrice());
  stopLimitMap->erase(limit->getStopPrice());
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
  if (node->rightLimit != nullptr) {
    node->rightLimit->parent = node;
  }

  newRoot->leftLimit = node;
  newRoot->parent = node->parent;
  node->parent = newRoot;
  updateHeight(node);
  updateHeight(newRoot);

  return newRoot;
}

Limit *OrderBook::rotateRight(Limit *node) {
  Limit *newRoot = node->rightLimit;
  node->leftLimit = newRoot->rightLimit;
  if (node->leftLimit != nullptr) {
    node->leftLimit->parent = node;
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
