#ifndef LIMIT_HPP
#define LIMIT_HPP

#include "../Order/order.h"

class Limit {

public: 
	Limit(int limitPrice);

	void addOrder(Order* order);

	int getVolume();
	void setVolume(int volume);

	Order* getHeadOrder();
	Order* getTailOrder();

	void execute(Order* headOrder, Order& order);
	void execute(int limitVolume, Order& order);
private: 
	int limitPrice;
	Limit* leftChildLimit = nullptr;
	Limit* rightChildLimit = nullptr;

	Order* headOrder = nullptr; 
	Order* tailOrder = nullptr;

	int volume = 0;
};

#endif
