#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include "tuple"
#include <random>

class OrderBook;

class OrderGenerator {
public:
	OrderGenerator(OrderBook* orderBook);

	void generateOrders(int noOfOrders);
private:
	OrderBook* orderBook;

	std::random_device rd;	
	std::mt19937 gen;

	std::tuple<int, int> sharesRange = {0, 1000};
	std::tuple<int, int> limitPriceRange = {50, 250};
	std::tuple<int, int> stopPriceRange = {50, 250};
};

#endif
