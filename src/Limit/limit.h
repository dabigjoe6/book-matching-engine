#ifndef LIMIT_HPP
#define LIMIT_HPP

class Order;
class OrderBook;

enum class LimitType {
	LimitBuy,
	LimitSell,
	StopBuy,
	StopSell
};

class Limit {

public: 
	Limit(OrderBook* orderBook, int price, LimitType type, Limit* parent = nullptr);

	int getPrice() const;

	void addOrder(Order* order);

	int getVolume() const;

	Order* getHeadOrder() const;
	Order* getTailOrder() const;

	void execute(Order* headOrder, Order* order);
private: 
	OrderBook* orderBook;
	
	int price;

	LimitType type;
	
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
