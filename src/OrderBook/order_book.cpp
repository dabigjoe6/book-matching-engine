#include <algorithm>

#include "order_book.h"

#include "../Limit/limit.h"
#include "../Order/order.h"

OrderBook::OrderBook() {}

void OrderBook::addOrder(Order& newOrder) {
	// market order
	// TODO: Verify this logic for market order makes sense
	// My assumption is if there's no limitPrice and stopPrice set we want to
	// execute the order immediately
	if (newOrder.getLimitPrice() == 0 && newOrder.getStopPrice() == 0) {
		addMarketOrder(newOrder);		}
	// limit order
	if (newOrder.getLimitPrice() > 0 && newOrder.getStopPrice() == 0) {
		addLimitOrder(newOrder);
	}
	// stop order or stop limit order
	if (newOrder.getStopPrice() > 0) {
		addStopOrder(newOrder);
	}
}

int OrderBook::marketOrderHelper(Limit* edge, Order& order) {
	while (
		((order.getLimitPrice() == 0 || (
			(order.getBuyOrSell() && order.getLimitPrice() >= edge->getLimitPrice()) || 
			(!order.getBuyOrSell() && order.getLimitPrice() <= edge->getLimitPrice()))
		) && 		
		(edge != nullptr && edge->getHeadOrder() != nullptr && order.getShares() != 0))
	)
	{
		edge->execute(edge->getHeadOrder(), order);
	}
	
	return order.getShares();
}

void OrderBook::executeStopOrders(int buyOrSell) {
	if (buyOrSell) {
		// TODO: COnfirm if predicate for making stop orders limit orders should be based on limit price or stop price
		while (lowestStopBuy != nullptr && (lowestSell == nullptr || lowestStopBuy->getLimitPrice() <= lowestSell->getLimitPrice())) {
			if (lowestStopBuy->getHeadOrder()->getLimitPrice() == 0) {
				int shares = marketOrderHelper(lowestSell, *(lowestStopBuy->getHeadOrder()));
				// TODO: Handle when shares != 0
				if (shares != 0) {

				}
				//TODO: Update lowestStopBuy -> Delete entire limit or just remove/change head order and tail order
			} else {
				addLimitOrder(*(lowestStopBuy->getHeadOrder()));
				//TODO: Update lowestStopBuy -> Delete entire limit or just remove/change head order and tail order

			}
		}
	} else {
		while (highestStopSell != nullptr && (highestBuy == nullptr || highestStopSell->getLimitPrice() >= highestBuy->getLimitPrice())) {
			if (highestStopSell->getHeadOrder()->getLimitPrice() == 0) {
				int shares = marketOrderHelper(highestBuy, *(highestStopSell->getHeadOrder()));
				// TODO: Handle when shares not equal zero	
				if (shares != 0) {

				}
				//TODO: Update lowestStopBuy -> Delete entire limit or just remove/change head order and tail order
			} else {
				addLimitOrder(*(highestStopSell->getHeadOrder()));
				//TODO: Update lowestStopBuy -> Delete entire limit or just remove/change head order and tail order

			}	
		}
	}
}

void OrderBook::addMarketOrder(Order& order) {
	Limit* edgeLimit = order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

	int shares = marketOrderHelper(edgeLimit, order);
	
	// TODO: Do we need to do anything with orders that weren't placed? i.e Add to Limit Order?
	if (shares != 0) {
		
	}

	executeStopOrders(order.getBuyOrSell());
}

void OrderBook::addStopOrderAsMarketOrLimitOrder(Limit* edgeLimit, Order& order) {

	// TODO: Confirm what happens when we can't fullfil the market order for the stop order
	// Do we make the unfullfilled ones limit orders? Or do they become stop orders?
	if (order.getBuyOrSell() && edgeLimit != nullptr && order.getStopPrice() <= edgeLimit->getLimitPrice()) {
		if (order.getLimitPrice() == 0) {
			int shares = marketOrderHelper(edgeLimit, order);
			// TODO: Handle when shares != 0	
			if (shares != 0) {
			}
		} else {
			addLimitOrder(order);	
		}
		return;
	} else if (!order.getBuyOrSell() && edgeLimit != nullptr && order.getStopPrice() >= edgeLimit->getLimitPrice()) {
		if (order.getLimitPrice() == 0) {
			int shares = marketOrderHelper(edgeLimit, order);
			// TODO: Handle when shares != 0	
			if (shares != 0) {

			}
		} else {
			addLimitOrder(order);
		}
		return;
	}

	return;
}

void OrderBook::addLimitOrder(Order& order) {
	Limit* edgeLimit = order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

	int shares = marketOrderHelper(edgeLimit, order);

	if (shares != 0) {
		int limitPrice = order.getLimitPrice();
		std::unordered_map<int, Limit*> limitMap = order.getBuyOrSell() ? buyLimitMap : sellLimitMap;

		if (limitMap.find(limitPrice) == limitMap.end()) {
			insertLimitIntoAVLTree(limitPrice, order.getBuyOrSell());
		}

		Limit* limit = limitMap[limitPrice];
		limit->addOrder(&order);

		return;
	}

	executeStopOrders(order.getBuyOrSell());
}

void OrderBook::addStopOrder(Order& order) {
	Limit* edgeLimit = order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();
	
	addStopOrderAsMarketOrLimitOrder(edgeLimit, order);

	if (order.getShares() != 0) {
		int stopPrice = order.getStopPrice();
		int limitPrice = order.getLimitPrice();

		std::unordered_map<int, Limit*> stopMap = order.getBuyOrSell() ? stopBuyMap : stopSellMap;

		if (stopMap.find(stopPrice) == stopMap.end()) {
			insertStopLimitIntoAVLTree(stopPrice, limitPrice, order.getBuyOrSell());
		}

		Limit* stopLimit = stopMap[stopPrice];
		stopLimit->addOrder(&order);	
		
		return;
	}
}

void OrderBook::insertLimitIntoAVLTree(int limitPrice, int buyOrSell) {
	Limit* tree = buyOrSell ? buyTree : sellTree;
	Limit* edge = buyOrSell ? highestBuy : lowestSell;
	std::unordered_map<int, Limit*> limitMap = buyOrSell ? buyLimitMap : sellLimitMap;
	
	if (tree == nullptr) {
		Limit* newLimit = new Limit(this, limitPrice);
		tree = newLimit;
		edge = tree;
		
		limitMap.try_emplace(limitPrice, newLimit);

		return;
	}

	Limit* newLimit = _insert(tree, limitPrice);
	limitMap.try_emplace(limitPrice, newLimit);
	updateBookEdgeOnInsert(newLimit, buyOrSell);
}

void OrderBook::updateBookEdgeOnInsert(Limit* newLimit, int buyOrSell) {	
	if (buyOrSell) {
		if (highestBuy != nullptr && newLimit->getLimitPrice() > highestBuy->getLimitPrice()) {
			highestBuy = newLimit;
		} else if (highestBuy == nullptr) {
			highestBuy = newLimit;
		}
	} else {
		if (lowestSell != nullptr && newLimit->getLimitPrice() < lowestSell->getLimitPrice()) {
			lowestSell = newLimit;
		} else if (lowestSell == nullptr) {
			lowestSell = newLimit;
		}
	}
}

void OrderBook::insertStopLimitIntoAVLTree(int stopPrice, int limitPrice, int buyOrSell) {
	Limit* tree = buyOrSell ? stopBuyTree : stopSellTree;
	Limit* edge = buyOrSell ? lowestStopBuy : highestStopSell;

	std::unordered_map<int, Limit*> stopMap = buyOrSell ? stopBuyMap : stopSellMap;

	if (tree == nullptr) {
		Limit* newStopLimit = new Limit(this, limitPrice, stopPrice);
		tree = newStopLimit;
		edge = tree;

		stopMap.try_emplace(stopPrice, newStopLimit);

		return;
	}

	Limit* newStopLimit = _insert(tree, stopPrice);
	stopMap.try_emplace(stopPrice, newStopLimit);
	updateBookStopEdgeOnInsert(newStopLimit, buyOrSell);
}

void OrderBook::updateBookStopEdgeOnInsert(Limit* newStopLimit, int buyOrSell) {
	Limit* edge = buyOrSell ? lowestStopBuy : highestStopSell;
	if (buyOrSell) {
		if (lowestStopBuy != nullptr && newStopLimit->getStopPrice() < lowestStopBuy->getStopPrice()) {
			lowestStopBuy = newStopLimit;
		} else if (lowestStopBuy == nullptr) {
			lowestStopBuy = newStopLimit;
		}
	} else {
		if (highestStopSell != nullptr && newStopLimit->getStopPrice() > highestStopSell->getStopPrice()) {
			highestStopSell = newStopLimit;
		} else if (highestStopSell == nullptr) {
			highestStopSell = newStopLimit;
		}
	}
}

Limit* OrderBook::_insert(Limit* root, int limitPrice) {
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

void OrderBook::deleteLimitFromAVLTree(Limit* limit, int buyOrSell) {
	Limit* tree = buyOrSell ? buyTree : sellTree;

	std::unordered_map<int, Limit*> limitMap = buyOrSell ? buyLimitMap : sellLimitMap;
	
	updateBookEdgeOnDelete(limit, buyOrSell);
	_delete(tree, limit->getLimitPrice());		
	limitMap.erase(limit->getLimitPrice());
}

void OrderBook::updateBookEdgeOnDelete(Limit* limit, int buyOrSell) {
	Limit* edge = buyOrSell ? highestBuy : lowestSell;

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

Limit* OrderBook::_delete(Limit* root, int limitPrice) {
	if (root == nullptr) {
		return root;
	}

	if (limitPrice < root->limitPrice) {
		root->leftLimit = _delete(root->leftLimit, limitPrice);
	} else if (limitPrice > root->limitPrice) {
		root->rightLimit = _delete(root->rightLimit, limitPrice);
	} else {
		if (root->leftLimit == nullptr) {
			Limit* temp = root->rightLimit;
			delete root;
			return temp;
		} else if (root->rightLimit == nullptr) {
			Limit* temp = root->leftLimit;
			delete root;
			return temp;
		}

		Limit* temp = getMinValueNode(root->rightLimit);
		root->limitPrice = temp->limitPrice;
		root->rightLimit = _delete(root->rightLimit, temp->limitPrice);
	}

	if (root == nullptr) return root;

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


Limit* OrderBook::getMinValueNode(Limit* node) {
	Limit* current = node;
	while (current->leftLimit != nullptr) {
		current = current->leftLimit;
	}
	return current;
}

Limit* OrderBook::rotateLeft(Limit* node) {
	Limit* newRoot = node->rightLimit;
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

Limit* OrderBook::rotateRight(Limit* node) {
	Limit* newRoot = node->rightLimit;
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

void OrderBook::updateHeight(Limit* root) {
	root->height = 1 + std::max(getHeight(root->rightLimit), getHeight(root->leftLimit));
}

int OrderBook::getHeight(Limit* limit) {
	if (limit == nullptr) return -1;

	return limit->height;
}

int OrderBook::getBalance(Limit* node) {
	if (node == nullptr) return 0;

	return getHeight(node->leftLimit) - getHeight(node->rightLimit);
}
