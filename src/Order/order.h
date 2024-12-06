#ifndef ORDER_HPP
#define ORDER_HPP

class Order {

public:
	Order(int shares, int buyOrSell, int limitPrice = 0, int stopPrice = 0);

	int getShares() const;
	void setShares(int shares);

	int getLimitPrice() const;
	int getStopPrice() const;
	int getBuyOrSell() const;

	Order* getNextOrder();
	Order* getPrevOrder();

	void setNextOrder(Order* order);
	void setPrevOrder(Order* order);

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
