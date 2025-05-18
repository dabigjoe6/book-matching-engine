#ifndef ORDER_HPP
#define ORDER_HPP

class Order {

public:
	Order(int id, int shares, int buy_or_sell, int limit_price = 0, int stop_price = 0);

	int get_id() const;

	int get_shares() const;
	int get_filled_shares() const;

	void set_shares(int shares);
	void set_filled_shares(int shares);

	int get_limit_price() const;
	int get_stop_price() const;
	int get_buy_or_sell() const;

	Order* get_next_order();
	Order* get_prev_order();

	void set_next_order(Order* order);
	void set_prev_order(Order* order);

private:
	int id;

	int shares = 0;
	int filled_shares = 0;

	int limit_price = 0;
	int stop_price = 0;

	int buy_or_sell;

	Order* next_order = nullptr;
	Order* prev_order = nullptr;

	friend class Limit;
};

#endif
