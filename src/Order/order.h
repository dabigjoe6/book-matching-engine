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
	int shares;
	int limitPrice;
	int stopPrice;

	int buyOrSell;

	Order* nextOrder = nullptr;
	Order* prevOrder = nullptr;
};

#endif
