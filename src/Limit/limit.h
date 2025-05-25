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
	explicit Limit(int _price, LimitType _type);

	void add_order(Order* order);

	int get_price() const;
	int get_shares() const;
	int get_volume() const;

	Order* get_head_order() const;
	Order* get_tail_order() const;

private: 
	int price;
	int shares;

	LimitType type;
	
	Order* head_order = nullptr; 
	Order* tail_order = nullptr;

	int volume = 0;

	friend class OrderBook;
};

#endif
