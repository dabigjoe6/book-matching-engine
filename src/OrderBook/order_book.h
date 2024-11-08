#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "../Limit/limit.h"

class OrderBook {
public:
	OrderBook();

	void addOrder(Order& newOrder);
	
	Limit* getHighestBuy();
	Limit* getLowestSell();

	Limit* getHighestStopSell();
	Limit* getLowestStopBuy();

private: 
	Limit* highestBuy;
	Limit* lowestSell;

	Limit* highestStopSell;
	Limit* lowestStopBuy;

	void addMarketOrder(Order& order);
	void addLimitOrder(Order& order);
	void addStopLimitOrder(Order& order);
	void addStopOrder(Order& order);

};

#endif
