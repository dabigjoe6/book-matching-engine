#include <iostream>
#include "./OrderBook/order_book.h"
#include "./OrderGenerator/order_generator.h"

using std::cout;

int main(int argc, char** argv) {
	cout << "Welcome to the Order Book" << "\n";

	OrderBook order_book = OrderBook();
	OrderGenerator generator = OrderGenerator(&order_book);

	generator.generateInitialOrders(4);
	generator.generateOrders(10);
	generator.simulateMarket();
}
