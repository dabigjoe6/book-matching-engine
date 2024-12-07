#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <random>
#include <tuple>
#include <fstream>
#include <string>
#include <vector>

class OrderBook;
class Order;

class OrderGenerator {
public:
	OrderGenerator(OrderBook* orderBook);
	~OrderGenerator();

	void openFile(std::string filePathString = "inital_orders.txt");

	void generateInitialOrders(int noOfOrders);
	void generateOrders(int noOfOrders);
	void simulateMarket();


private:
	OrderBook* orderBook;

	std::random_device rd;	
	std::mt19937 gen;

	std::ofstream file;	

	std::tuple<int, int> sharesRange = {0, 1000};
	std::tuple<int, int> limitPriceRange = {50, 250};
	std::tuple<int, int> stopPriceRange = {50, 250};


	Order* generateOrder();
	std::vector<Order*> readOrders();
	void processOrders(std::vector<Order*> readOrders);
};

#endif
