#include <iostream>
#include "./Order/order.h"
#include "./OrderBook/order_book.h"

using std::cout;

int main(int argc, char** argv) {
	cout << "Welcome to the Order Book" << "\n";

	Order new_order = Order(10, 1, 0);
	OrderBook order_book = OrderBook();
	order_book.addOrder(new_order);
}
