#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <unordered_map>

class Limit;
class Order;

class OrderBook {
public:
	OrderBook();

	void addOrder(Order& newOrder);
	
	Limit* getHighestBuy();
	Limit* getLowestSell();

	Limit* getHighestStopSell();
	Limit* getLowestStopBuy();

private:
	Limit* buyTree = nullptr;
	Limit* sellTree = nullptr;

	Limit* stopBuyTree;
	Limit* stopSellTree;

	Limit* highestBuy;
	Limit* lowestSell;

	Limit* highestStopSell;
	Limit* lowestStopBuy;

	std::unordered_map<int, Limit*> buyLimitMap;
	std::unordered_map<int, Limit*> sellLimitMap;

	void addMarketOrder(Order& order);
	void addLimitOrder(Order& order);
	void addStopLimitOrder(Order& order);
	void addStopOrder(Order& order);

	void marketOrderHelper(Limit* limit, Order& order);

	void insertLimitIntoAVLTree(int limitPrice, int buyOrSell);
	Limit* _insert(Limit* root, int limitPrice);
	void updateHeight(Limit* root);
	int getHeight(Limit* node);
	int getBalance(Limit* node);
	Limit* rotateLeft(Limit* root);
	Limit* rotateRight(Limit* root);
};

#endif
