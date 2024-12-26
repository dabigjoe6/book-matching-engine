#ifndef LIMIT_HPP
#define LIMIT_HPP

class Order;
class OrderBook;

class Limit {

public: 
	Limit(OrderBook* orderBook, int limitPrice, int stopPrice = 0, Limit* parent = nullptr);
	Limit(OrderBook* orderBook, int limitPrice, Limit* parent = nullptr);


	int getLimitPrice() const;
	int getStopPrice() const;

	void addOrder(Order* order);

	int getVolume() const;

	Order* getHeadOrder() const;
	Order* getTailOrder() const;

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
