#include "./OrderBook/order_book.h"
#include "./OrderGenerator/order_generator.h"
#include <iostream>

using std::cout;

int main(int argc, char **argv) {
  cout << "Welcome to the Order Book" << "\n";

  OrderBook order_book = OrderBook();
  OrderGenerator generator = OrderGenerator(&order_book);

  generator.generateOrders("initial_orders.txt", 4);
  generator.generateOrders("orders.txt", 10);
  generator.simulateMarket();
}
