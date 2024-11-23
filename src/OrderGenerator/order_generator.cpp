#include <random>
#include <iostream>
#include <fstream>

#include "order_generator.h"
#include "../OrderBook/order_book.h"
#include "../Order/order.h"

namespace fs = std::filesystem;

OrderGenerator::OrderGenerator(OrderBook* orderBook): orderBook(orderBook), gen(rd()) {
	fs::path filePath = fs::current_path() / "orders.txt";
	file.open(filePath);

	if (!file.is_open()) {
		std::cerr << "Could not open file for writing!" << std::endl;	
	}
}

OrderGenerator::~OrderGenerator() {
	if (file.is_open()) {
		file.close();
		std::cout << "file closed" << std::endl;
	}
}

void OrderGenerator::generateInitialOrders(int noOfOrders) {
	std::uniform_int_distribution<> sharesDist(std::get<0>(sharesRange), std::get<1>(sharesRange));
	std::uniform_int_distribution<> buyOrSellDist(0, 1);
	std::uniform_int_distribution<> limitPriceDist(std::get<0>(limitPriceRange), std::get<1>(limitPriceRange));
	std::uniform_int_distribution<> stopPriceDist(std::get<0>(stopPriceRange), std::get<1>(stopPriceRange));
	std::uniform_int_distribution<> isStopPriceOrderDist(0, 1);

	for (int i = 0; i < noOfOrders; ++i) {
		int shares = sharesDist(gen);
		int buyOrSell = buyOrSellDist(gen);
		int limitPrice = limitPriceDist(gen);
		
		int isStopPriceOrder = isStopPriceOrderDist(gen);
		int stopPrice = 0;
		if (isStopPriceOrder) {
			stopPrice = stopPriceDist(gen);
		}

		Order* newOrder = new Order(shares, buyOrSell, limitPrice, stopPrice);
		if (stopPrice == 0) {
			orderBook->queueOrderInLimit(*newOrder);
		} else {
			orderBook->queueStopOrderInLimit(*newOrder);
		}

		file << "Order " << shares << "\t" << buyOrSell << "\t" << limitPrice << "\t" << stopPrice << "\n"; 
	}
}
