#include <iostream>
#include <algorithm>

#include "order_book.h"

#include "../Limit/limit.h"
#include "../Order/order.h"

using std::cout;

OrderBook::OrderBook() {
	cout << "Order book works" << "\n";
}

void OrderBook::addOrder(Order& newOrder) {
	// market order
	// TODO: Verify this logic for market order makes sense
	// My assumption is if there's no limitPrice and stopPrice set we want to
	// execute the order immediately
	if (newOrder.getLimitPrice() == 0 && newOrder.getStopPrice() == 0) {
		addMarketOrder(newOrder);		
	}

	// limit order
	if (newOrder.getLimitPrice() > 0 && newOrder.getStopPrice() == 0) {
		addLimitOrder(newOrder);
	}

	// stop order
	if (newOrder.getLimitPrice() == 0 && newOrder.getStopPrice() > 0) {
		addStopOrder(newOrder);
	}
	
	// stop limit order
	if (newOrder.getLimitPrice() > 0 && newOrder.getStopPrice() > 0) {
		addStopLimitOrder(newOrder);
	}
}

void OrderBook::marketOrderHelper(Limit* limit, Order& order) {
	if (limit != nullptr && limit->getVolume() > order.getShares()) {
		// TODO: Confirm if we need to check if there's a limit on the head Order before
		// execution
	
		//TODO:  Since the volume of the Limit is greater than the number of shares
		// We might need to partially feel some orders in the Limit
		// Should we check for this in execute? 
		while (limit->getHeadOrder() != nullptr) {
			limit->execute(limit->getHeadOrder(), order);
		}
		
		//TODO: If above comment on limit on head Order is true, we might need to
		// check if there are still shares left to execute given, we might not execute 
		// all at once
		if (order.getShares() != 0) {
			// TODO: Add to Limit Order?
			// What will be the limit price? or do we just discard it? 
		}

		return;
	}

	if (limit != nullptr && limit->getVolume() <= order.getShares()) {
		//TODO: Confirm if we need to check if there's a limit on the head Order before
		// execution
		
		limit->execute(limit->getVolume(), order); // Since order is more than or equal to entire volume of limit. Execute entire volume at once. TODO: Confirm if this is correct i.e we don't need to check for the limit of each order in the limit list
		
		
		//TODO: If above comment on limit on head Order is true, we might need to
		// check if there are still shares left to execute given, we might not execute 
		// all at once
		if (order.getShares() != 0) {
			// TODO: Add to Limit Order?
			// What will be the limit price? or do we just discard it? 
		}
	}
}

void OrderBook::addMarketOrder(Order& order) {
	Limit* oppositeLimit = order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

	marketOrderHelper(oppositeLimit, order);
	
	// TODO: Do we need to do anything with orders that weren't placed? i.e Add to Limit Order?
	if (order.getShares() != 0) {
		
	}
}

void OrderBook::addLimitOrder(Order& order) {
	Limit* oppositeLimit = order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

	marketOrderHelper(oppositeLimit, order);

	if (order.getShares() != 0) {
		int limitPrice = order.getLimitPrice();
		std::unordered_map<int, Limit*> limitMap = order.getBuyOrSell() ? buyLimitMap : sellLimitMap;

		if (limitMap.find(limitPrice) == limitMap.end()) {
			insertLimitIntoAVLTree(limitPrice, order.getBuyOrSell());
		}

		Limit* limit = limitMap[limitPrice];
		limit->addOrder(&order);

		return;
	}
}

void OrderBook::insertLimitIntoAVLTree(int limitPrice, int buyOrSell) {
	Limit* tree = buyOrSell ? buyTree : sellTree;
	Limit* edge = buyOrSell ? highestBuy : lowestSell;
	std::unordered_map<int, Limit*> limitMap = buyOrSell ? buyLimitMap : sellLimitMap;
	
	if (tree == nullptr) {
		Limit* newLimit = new Limit(limitPrice);
		tree = newLimit;
		edge = tree;
		
		limitMap.try_emplace(limitPrice, newLimit);

		return;
	}

	Limit* newLimit = _insert(tree, limitPrice);
	limitMap.try_emplace(limitPrice, newLimit);
}

Limit* OrderBook::_insert(Limit* root, int limitPrice) {
	if (root == nullptr) {
		return new Limit(limitPrice);
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
