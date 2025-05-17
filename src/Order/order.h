#ifndef ORDER_HPP
#define ORDER_HPP

class Order {

public:
	Order(int id, int shares, int buyOrSell, int limitPrice = 0, int stopPrice = 0);

	int getId() const;

	int getShares() const;
	int getFilledShares() const;

	void setShares(int shares);
	void setFilledShares(int shares);

	int getLimitPrice() const;
	int getStopPrice() const;
	int getBuyOrSell() const;

	Order* getNextOrder();
	Order* getPrevOrder();

	void setNextOrder(Order* order);
	void setPrevOrder(Order* order);

private:
	int id;

	int shares = 0;
	int filledShares = 0;

	int limitPrice = 0;
	int stopPrice = 0;

	int buyOrSell;

	Order* nextOrder = nullptr;
	Order* prevOrder = nullptr;

	friend class Limit;
};

#endif
