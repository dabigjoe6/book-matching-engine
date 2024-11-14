#include <stdexcept>
#include "limit.h"
#include "../Order/order.h"
#include "../OrderBook/order_book.h"

Limit::Limit(OrderBook* orderBook, int limitPrice, int stopPrice): orderBook(orderBook), limitPrice(limitPrice), stopPrice(stopPrice) {
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

int Limit::getVolume() { return volume; }

Order* Limit::getHeadOrder() { return headOrder; }
Order* Limit::getTailOrder() { return tailOrder; }


//TODO: Confirm cleanup of order
void Limit::execute(Order* headOrder, Order& order) {
	if (headOrder->getShares() <= order.getShares()) {
		this->volume -= headOrder->getShares();
		order.setShares(order.getShares() - headOrder->getShares());	

		Order* tempOrder = headOrder;
		headOrder = headOrder->nextOrder;
		if (headOrder != nullptr) {
			headOrder->prevOrder = nullptr;

			delete tempOrder;
			return;
		} else {
			orderBook->deleteLimitFromAVLTree(this, !order.getBuyOrSell());
		}
	}

	this->volume -= order.getShares();
	headOrder->setShares(headOrder->getShares() - order.getShares());
	return;
}
