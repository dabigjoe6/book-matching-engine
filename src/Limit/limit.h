#ifndef LIMIT_HPP
#define LIMIT_HPP

#include "../AVLTree/avl_node.h"

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
	explicit Limit(int _price, LimitType _type, Order* order=nullptr);

	void add_order(Order* order);

	int get_price() const;
	int get_volume() const;

	Order* get_head_order() const;
	Order* get_tail_order() const;

private: 
	int price;
	int volume = 0;

	LimitType type;
	
	Order* head_order = nullptr; 
	Order* tail_order = nullptr;


	friend class OrderBook;
};

#endif
