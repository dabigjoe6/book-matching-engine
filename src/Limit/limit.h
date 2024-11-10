#ifndef LIMIT_HPP
#define LIMIT_HPP

class Order;

class Limit {

public: 
	Limit(int limitPrice, int stopPrice = 0);

	int getLimitPrice();
	int getStopPrice();

	void addOrder(Order* order);

	int getVolume();
	void setVolume(int volume);

	Order* getHeadOrder();
	Order* getTailOrder();

	void execute(Order* headOrder, Order& order);
	void execute(int limitVolume, Order& order);
private: 
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
