#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "../Order/order.h"
#include "../OrderBook/order_book.h"
#include "order_generator.h"

namespace fs = std::filesystem;

OrderGenerator::OrderGenerator(OrderBook *orderBook)
    : orderBook(orderBook), gen(rd()) {}

OrderGenerator::~OrderGenerator() {
  if (file.is_open()) {
    file.close();
    std::cout << "file closed" << std::endl;
  }
}

void OrderGenerator::openFile(std::string filePathString) {
  file.close();
  fs::path filePath = fs::current_path() / filePathString;
  file.open(filePath);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
}

void OrderGenerator::generateOrders(const std::string &file_name,
                                    const int &noOfOrders) {
  try {
    openFile(file_name);
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    file.clear();
    return;
  }

  for (int i = 0; i < noOfOrders; ++i) {
    Order *newOrder = generateOrder();
    file << newOrder->getId() << "\t" << newOrder->getShares() << "\t"
         << newOrder->getBuyOrSell() << "\t" << newOrder->getLimitPrice()
         << "\t" << newOrder->getStopPrice() << "\n"
         << std::flush;
  }
}

Order *OrderGenerator::generateOrder() {
  std::uniform_int_distribution<> idDistribution(std::get<0>(idRange), std::get<1>(idRange));
  std::uniform_int_distribution<> sharesDist(std::get<0>(sharesRange),
                                             std::get<1>(sharesRange));
  std::uniform_int_distribution<> buyOrSellDist(0, 1);
  std::uniform_int_distribution<> limitPriceDist(std::get<0>(limitPriceRange),
                                                 std::get<1>(limitPriceRange));
  std::uniform_int_distribution<> stopPriceDist(std::get<0>(stopPriceRange),
                                                std::get<1>(stopPriceRange));
  std::uniform_int_distribution<> isStopPriceOrderDist(0, 1);


  int id = idDistribution(gen);
  int shares = sharesDist(gen);
  int buyOrSell = buyOrSellDist(gen);
  int limitPrice = limitPriceDist(gen);

  int isStopPriceOrder = isStopPriceOrderDist(gen);
  int stopPrice = 0;
  if (isStopPriceOrder) {
    stopPrice = stopPriceDist(gen);
  }

  return new Order(id, shares, buyOrSell, limitPrice, stopPrice);
}

void OrderGenerator::simulateMarket() {
  // read and parse orders from orders.txt
  processInitialOrders(readOrders("initial_orders.txt"));
  processOrders(readOrders("orders.txt"));
}

void OrderGenerator::processInitialOrders(
    const std::vector<Order *> &readOrders) {
  for (Order *order : readOrders) {
    if (order->getStopPrice() == 0) {
      orderBook->addLimitOrderToLimitQueue(*order);
    } else {
      orderBook->addStopOrderToStopQueue(*order);
    }
  }
}
void OrderGenerator::processOrders(const std::vector<Order *> &readOrders) {
  for (Order *order : readOrders) {
    orderBook->addOrder(order);
  }
}

std::vector<Order *> OrderGenerator::readOrders(const std::string &file_name) {
  std::ifstream inputFile(fs::current_path() / file_name);
  if (!inputFile.is_open()) {
    std::cerr << "Could not read orders" << std::endl;
    return {};
  }


  // TODO: Do allocation here
  std::vector<Order *> readOrders;

  std::string line;
  int i_ran_at_least_once = 0;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);

    i_ran_at_least_once += 1;

    int orderId;
    iss >> orderId;

    int shares;
    iss >> shares;

    int buyOrSell;
    iss >> buyOrSell;

    int limitPrice;
    iss >> limitPrice;

    int stopPrice;
    iss >> stopPrice;

    Order *readOrder = new Order(shares, buyOrSell, limitPrice, stopPrice);
    readOrders.emplace_back(readOrder);
  }

  return readOrders;
}
