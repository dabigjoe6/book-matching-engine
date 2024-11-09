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
	
	// TODO: Do we need to do anything with orders that weren't placed? i.e Add to Limit Order?
	if (order.getShares() != 0) {
		
	}
}



