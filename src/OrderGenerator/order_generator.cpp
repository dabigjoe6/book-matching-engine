#include <random>
#include <string>
#include <iostream>
#include <fstream>

#include "order_generator.h"
#include "../OrderBook/order_book.h"
#include "../Order/order.h"

namespace fs = std::filesystem;

OrderGenerator::OrderGenerator(OrderBook* orderBook): orderBook(orderBook), gen(rd()) {
	openFile();
}

OrderGenerator::~OrderGenerator() {
	if (file.is_open()) {
		file.close();
		std::cout << "file closed" << std::endl;
	}
}

void OrderGenerator::openFile(std::string filePathString) {
	fs::path filePath = fs::current_path() / filePathString;
	file.open(filePath);

	if (!file.is_open()) {
		std::cerr << "Could not open file for writing!" << std::endl;	
	}
}

void OrderGenerator::generateInitialOrders(int noOfOrders) {
	for (int i = 0; i < noOfOrders; ++i) {
		Order* newOrder = generateOrder();
		if (newOrder->getStopPrice() == 0) {
			orderBook->queueOrderInLimit(*newOrder);
		} else {
			orderBook->queueStopOrderInLimit(*newOrder);
		}

		file << "Order " << newOrder->getShares() << "\t" << newOrder->getBuyOrSell() << "\t" << newOrder->getLimitPrice() << "\t" << newOrder->getStopPrice() << "\n"; 
	}
}

void OrderGenerator::generateOrders(int noOfOrders) {
	for (int i = 0; i < noOfOrders; ++i) {
		Order* newOrder = generateOrder();
		file << "Order " << newOrder->getShares() << "\t" << newOrder->getBuyOrSell() << "\t" << newOrder->getLimitPrice() << "\t" << newOrder->getStopPrice() << "\n"; 	
	}
}

Order* OrderGenerator::generateOrder() {
	std::uniform_int_distribution<> sharesDist(std::get<0>(sharesRange), std::get<1>(sharesRange));
	std::uniform_int_distribution<> buyOrSellDist(0, 1);
	std::uniform_int_distribution<> limitPriceDist(std::get<0>(limitPriceRange), std::get<1>(limitPriceRange));
	std::uniform_int_distribution<> stopPriceDist(std::get<0>(stopPriceRange), std::get<1>(stopPriceRange));
	std::uniform_int_distribution<> isStopPriceOrderDist(0, 1);

	int shares = sharesDist(gen);
	int buyOrSell = buyOrSellDist(gen);
	int limitPrice = limitPriceDist(gen);
	
	int isStopPriceOrder = isStopPriceOrderDist(gen);
	int stopPrice = 0;
	if (isStopPriceOrder) {
		stopPrice = stopPriceDist(gen);
	}

	return new Order(shares, buyOrSell, limitPrice, stopPrice);
}

void OrderGenerator::simulateMarket() {

}

