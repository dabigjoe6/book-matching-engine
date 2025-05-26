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

OrderGenerator::OrderGenerator(OrderBook *order_book)
    : order_book(order_book), gen(rd()) {}

OrderGenerator::~OrderGenerator() {
  if (file.is_open()) {
    file.close();
    std::cout << "file closed" << std::endl;
  }
}

void OrderGenerator::open_file(std::string file_path_string) {
  file.close();
  fs::path file_path = fs::current_path() / file_path_string;
  file.open(file_path);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
}

void OrderGenerator::generate_orders(const std::string &file_name,
                                    const int &no_of_orders) {
  try {
    open_file(file_name);
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    file.clear();
    return;
  }

  for (int i = 0; i < no_of_orders; ++i) {
    Order *newOrder = generate_order();
    file << newOrder->get_id() << "\t" << newOrder->get_shares() << "\t"
         << newOrder->get_buy_or_sell() << "\t" << newOrder->get_limit_price()
         << "\t" << newOrder->get_stop_price() << "\n"
         << std::flush;
  }
}

Order *OrderGenerator::generate_order() {
  std::uniform_int_distribution<> id_distribution(std::get<0>(id_range), std::get<1>(id_range));
  std::uniform_int_distribution<> shares_distribution(std::get<0>(shares_range),
                                             std::get<1>(shares_range));
  std::uniform_int_distribution<> buy_or_sell_distribution(0, 1);
  std::uniform_int_distribution<> limit_price_distribution(std::get<0>(limit_price_range),
                                                 std::get<1>(limit_price_range));
  std::uniform_int_distribution<> stop_price_distribution(std::get<0>(stop_price_range),
                                                std::get<1>(stop_price_range));
  std::uniform_int_distribution<> is_stop_price_order_distribution(0, 1);


  int id = id_distribution(gen);
  int shares = shares_distribution(gen);
  bool buy_or_sell = buy_or_sell_distribution(gen);
  int limit_price = limit_price_distribution(gen);

  int is_stop_price_order = is_stop_price_order_distribution(gen);
  int stop_price = 0;
  if (is_stop_price_order) {
    stop_price = stop_price_distribution(gen);
  }

  return new Order(id, shares, buy_or_sell, limit_price, stop_price);
}

void OrderGenerator::simulate_market() {
  // read and parse orders from orders.txt
  process_initial_orders(read_orders("initial_orders.txt"));
  process_orders(read_orders("orders.txt"));
}

void OrderGenerator::process_initial_orders(
    const std::vector<Order *> &read_orders) {
  for (Order *order : read_orders) {
    if (order->get_stop_price() == 0 && order->get_limit_price() != 0) {
      order_book->add_limit_order(order);
    } else {
      order_book->add_stop_order(order);
    }
  }
}

void OrderGenerator::process_orders(const std::vector<Order *> &read_orders) {
  for (Order *order : read_orders) {
    order_book->place_order(order);
  }
}

std::vector<Order *> OrderGenerator::read_orders(const std::string &file_name) {
  std::ifstream input_file(fs::current_path() / file_name);
  if (!input_file.is_open()) {
    std::cerr << "Could not read orders" << std::endl;
    return {};
  }


  // TODO: Do allocation here
  std::vector<Order *> read_orders;

  std::string line;
  int i_ran_at_least_once = 0;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);

    i_ran_at_least_once += 1;

    int order_id;
    iss >> order_id;

    int shares;
    iss >> shares;

    bool buy_or_sell;
    iss >> buy_or_sell;

    int limit_price;
    iss >> limit_price;

    int stop_price;
    iss >> stop_price;

    Order *read_order = new Order(order_id, shares, buy_or_sell, limit_price, stop_price);
    read_orders.emplace_back(read_order);
  }

  return read_orders;
}
