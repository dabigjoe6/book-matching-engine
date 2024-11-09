#ifndef LIMIT_HPP
#define LIMIT_HPP

class Order;

class Limit {

public: 
	Limit(int limitPrice);

	int getLimitPrice();

	void addOrder(Order* order);

	int getVolume();
	void setVolume(int volume);

	Order* getHeadOrder();
	Order* getTailOrder();

	void execute(Order* headOrder, Order& order);
	void execute(int limitVolume, Order& order);
private: 
	int limitPrice;
	
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
