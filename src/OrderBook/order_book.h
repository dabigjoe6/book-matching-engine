#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <unordered_map>

class Limit;
class Order;
class OrderGenerator;

class OrderBook {
public:
	OrderBook();

	void addOrder(Order& newOrder);
	
	Limit* getHighestBuy();
	Limit* getLowestSell();

	Limit* getHighestStopSell();
	Limit* getLowestStopBuy();

	void deleteLimitFromAVLTree(Limit* limit, int buyOrSell);
	void deleteStopLimitFromAVLTree(Limit* limit, int buyOrSell);

private:
	Limit* buyTree = nullptr;
	Limit* sellTree = nullptr;

	Limit* stopBuyTree = nullptr;
	Limit* stopSellTree = nullptr;

	Limit* highestBuy;
	Limit* lowestSell;

	Limit* highestStopSell;
	Limit* lowestStopBuy;

	std::unordered_map<int, Limit*>* buyLimitMap;
	std::unordered_map<int, Limit*>* sellLimitMap;

	std::unordered_map<int, Limit*>* stopBuyMap;
	std::unordered_map<int, Limit*>* stopSellMap;

	void addMarketOrder(Order& order);
	void addLimitOrder(Order& order);
	void addStopOrder(Order& order);

	void queueOrderInLimit(Order& order);
	void queueStopOrderInLimit(Order& order);

	int marketOrderHelper(Limit* limit, Order& order);
	bool addStopOrderAsMarketOrLimitOrder(Limit* edgeLimit, Order& order);

	void executeStopOrders(int buyOrSell);

	void insertLimitIntoAVLTree(const int limitPrice, const int buyOrSell);
	void insertStopLimitIntoAVLTree(const int stopPrice, const int limitPrice, const int buyOrSell);

	void updateBookEdgeOnInsert(Limit* newLimit, const int buyOrSell);
	void updateBookStopEdgeOnInsert(Limit* newLimit, const int buyOrSell);
	
	void updateBookEdgeOnDelete(Limit* limit, const int buyOrSell);
	void updateBookStopEdgeOnDelete(Limit* limit, const int buyOrSell);

	Limit* _insert(Limit* root, const int limitPrice);
	Limit* _insert(Limit* root, const int stopPrice, const int limitPrice);

	Limit* _delete(Limit* root, const int limitPrice);
	Limit* _stopDelete(Limit* root, const int stopPrice);

	void updateHeight(Limit* root);

	int getHeight(const Limit* node);
	int getBalance(const Limit* node);

	Limit* getMinValueNode(Limit* node);

	Limit* rotateLeft(Limit* root);

	Limit* rotateRight(Limit* root);

	friend OrderGenerator;
};

#endif
