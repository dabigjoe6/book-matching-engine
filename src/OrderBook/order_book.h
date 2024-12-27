#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <unordered_map>

class Limit;
class Order;
class OrderGenerator;

enum class LimitType;

class OrderBook {
public:
  OrderBook();

  void addOrder(Order &newOrder);

  Limit *getHighestBuy();
  Limit *getLowestSell();

  Limit *getHighestStopSell();
  Limit *getLowestStopBuy();

  void deleteLimitFromAVLTree(Limit *limit, int buyOrSell);
  void deleteStopLimitFromAVLTree(Limit *limit, int buyOrSell);

private:
  Limit *buyTree = nullptr;
  Limit *sellTree = nullptr;

  Limit *stopBuyTree = nullptr;
  Limit *stopSellTree = nullptr;

  Limit *highestBuy;
  Limit *lowestSell;

  Limit *highestStopSell;
  Limit *lowestStopBuy;

  std::unordered_map<int, Limit *> _buyLimitMap = {};
  std::unordered_map<int, Limit *> *buyLimitMap = &_buyLimitMap;

  std::unordered_map<int, Limit *> _sellLimitMap = {};
  std::unordered_map<int, Limit *> *sellLimitMap = &_sellLimitMap;

  std::unordered_map<int, Limit *> _stopBuyMap = {};
  std::unordered_map<int, Limit *> *stopBuyMap = &_stopBuyMap;

  std::unordered_map<int, Limit *> _stopSellMap = {};
  std::unordered_map<int, Limit *> *stopSellMap = &_stopSellMap;

  void addMarketOrder(Order &order);
  void addLimitOrder(Order &order);
  void addStopOrder(Order &order);

  void addLimitOrderToLimitQueue(Order &order);
  void addStopOrderToStopQueue(Order &order);

  int marketOrderHelper(Limit *limit, Order &order);
  bool addStopOrderAsMarketOrLimitOrder(Limit *edgeLimit, Order &order);

  void executeStopOrders(int buyOrSell);

  void insertLimitIntoAVLTree(const int price, const int buyOrSell);
  void insertStopLimitIntoAVLTree(const int price, const int buyOrSell);

  void updateBookEdgeOnInsert(Limit *newLimit, const LimitType& type);
  void updateBookEdgeOnDelete(Limit *limit, const LimitType& type);

  Limit *_insert(Limit *root, const int &price, const LimitType &type,
                 std::unordered_map<int, Limit *> *limitMap,
                 Limit *parent);

  Limit *_delete(Limit *root, const int limitPrice);
  Limit *_stopDelete(Limit *root, const int stopPrice);

  void updateHeight(Limit *root);

  int getHeight(const Limit *node);
  int getBalance(const Limit *node);

  Limit *getMinValueNode(Limit *node);

  Limit *rotateLeft(Limit *root);

  Limit *rotateRight(Limit *root);

  friend OrderGenerator;
};

#endif
