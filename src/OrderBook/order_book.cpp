#include <iostream>
#include "order_book.h"


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

void OrderBook::addMarketOrder(Order& order) {
	Limit* limit = order.getBuyOrSell() ? this->getLowestSell() : this->getHighestBuy();

	if (limit != nullptr && limit->getVolume() > order.getShares()) {
		while (limit->getHeadOrder() != nullptr) {
			limit->execute(limit->getHeadOrder(), order);
		}

		if (order.getShares() != 0) {
			// TODO: You stopped here
			
		}

		return;
	}



}
