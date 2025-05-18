#ifndef LIMIT_HPP
#define LIMIT_HPP

#include "avl_node.h"

class Order;
class OrderBook;

enum class LimitType {
	LimitBuy,
	LimitSell,
	StopBuy,
	StopSell
};

class Limit: public Node<Limit, int> {

public: 
	Limit(int price, LimitType type);

	int get_price() const;

	void add_order(Order* order);

	int get_volume() const;

	Order* get_head_order() const;
	Order* get_tail_order() const;

private: 
	int price;

	LimitType type;
	
	Order* headOrder = nullptr; 
	Order* tailOrder = nullptr;

	int volume = 0;

	friend class OrderBook;
};

#endif
