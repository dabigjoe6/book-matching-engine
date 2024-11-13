#include <stdexcept>
#include "limit.h"
#include "../Order/order.h"

Limit::Limit(int limitPrice, int stopPrice): limitPrice(limitPrice), stopPrice(stopPrice) {
}

int Limit::getLimitPrice() { return limitPrice; }

int Limit::getStopPrice() { return stopPrice; }

void Limit::addOrder(Order* order) {
	if (order == nullptr) {
		throw std::invalid_argument("Order can not be a nullptr");

	}

	if (headOrder == nullptr) {
		headOrder = tailOrder = order;
	} else {
		tailOrder->nextOrder = order;
		order->prevOrder = tailOrder;
		tailOrder = order;
	}

	volume += order->getShares(); // TODO: Confirm this is the correct representation of limit volume
}


