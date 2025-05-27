#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <fstream>
#include <random>
#include <string>
#include <tuple>
#include <vector>

class OrderBook;
class Order;

class OrderGenerator {
public:
  OrderGenerator(OrderBook *order_book);
  ~OrderGenerator();

  void open_file(std::string file_path_string);

  void generate_orders(const std::string &file_name, const int &no_of_orders);
  void simulate_market();

private:
  OrderBook *order_book;

  std::random_device rd;
  std::mt19937 gen;

  std::ofstream file;

  std::tuple<int, int> id_range = {100000, 999999}; 
  std::tuple<int, int> shares_range = {1, 1000};
  std::tuple<int, int> limit_price_range = {50, 250};
  std::tuple<int, int> stop_price_range = {50, 250};

  Order *generate_order();
  std::vector<Order *> read_orders(const std::string &file_name);

  void process_initial_orders(const std::vector<Order *> &read_orders);
  void process_orders(const std::vector<Order *> &read_orders);
};

#endif
