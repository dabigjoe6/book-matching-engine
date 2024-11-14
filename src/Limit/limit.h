#ifndef LIMIT_HPP
#define LIMIT_HPP

class Order;
class OrderBook;

class Limit {

public: 
	Limit(OrderBook* orderBook, int limitPrice, int stopPrice = 0);

	int getLimitPrice();
	int getStopPrice();

	void addOrder(Order* order);

	int getVolume();

	Order* getHeadOrder();
	Order* getTailOrder();

	void execute(Order* headOrder, Order& order);
private: 
	OrderBook* orderBook;

	int limitPrice;
	int stopPrice;
	
	Order* headOrder = nullptr; 
	Order* tailOrder = nullptr;

	int volume = 0;

	Limit* parent = nullptr;
	Limit* leftLimit = nullptr;
	Limit* rightLimit = nullptr;

	int height = 0;

	friend class OrderBook;
};

#endif
