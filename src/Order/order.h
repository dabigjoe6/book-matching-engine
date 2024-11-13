#ifndef ORDER_HPP
#define ORDER_HPP

class Order {

public:
	Order(int shares, int buyOrSell, int limitPrice = 0, int stopPrice = 0);

	int getShares();
	void setShares();

	int getLimitPrice();
	int getStopPrice();
	int getBuyOrSell();

private:
	int shares = 0;
	int limitPrice = 0;
	int stopPrice = 0;

	int buyOrSell;

	Order* nextOrder = nullptr;
	Order* prevOrder = nullptr;

	friend class Limit;
};

#endif
