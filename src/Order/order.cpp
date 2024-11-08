#include <iostream>
#include "order.h"


using std::cout;


Order::Order(int shares, int buyOrSell, int limitPrice) {
	cout << "Order works: " << shares << buyOrSell << limitPrice << "\n";
}
