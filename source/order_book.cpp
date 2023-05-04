#include "order_book.hpp"
#include "matching_engine.hpp"
#include "order.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <string>

const auto OrderBook::EMPTY_BID_ORDER = std::string(Order::ID_WIDTH, ' ') + "|" +
										std::string(Order::VOL_WIDTH, ' ') + "|" +
										std::string(Order::PRICE_WIDTH, ' ');
const auto OrderBook::EMPTY_ASK_ORDER =
	std::string(OrderBook::EMPTY_BID_ORDER.rbegin(), OrderBook::EMPTY_BID_ORDER.rend());

OrderBook::OrderBook() : m_matching_engine{*this} {
}

auto OrderBook::add_order(const std::shared_ptr<Order>& order) -> void {
	m_matching_engine.match(order);

	if (order->get_quantity() > 0) {
		auto limit = order->get_price();

		if (order->is_bid()) {
			m_bids[limit].push_back(order);
		} else {
			m_asks[limit].push_back(order);
		}
	}
}

auto OrderBook::get_bid_tree() -> bid_tree_t& {
	return m_bids;
}

auto OrderBook::get_ask_tree() -> ask_tree_t& {
	return m_asks;
}

auto operator<<(std::ostream& ostream, const OrderBook& order_book) -> std::ostream& {
	ostream << "+-----------------------------------------------------------------+\n"
			<< "| BUY                            | SELL                           |\n"
			<< "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
			<< "+----------+-------------+-------+-------+-------------+----------+\n";

	auto bid_itr = OrderIterator<OrderBook::bid_tree_t>(order_book.m_bids);
	auto ask_itr = OrderIterator<OrderBook::ask_tree_t>(order_book.m_asks);

	while (!bid_itr.is_done() || !ask_itr.is_done()) {
		if (bid_itr.get_order() != nullptr) {
			std::cout << "|" << *bid_itr.get_order() << "|";
			bid_itr.next();
		} else {
			std::cout << "|" << OrderBook::EMPTY_BID_ORDER << "|";
		}

		if (ask_itr.get_order() != nullptr) {
			std::cout << *ask_itr.get_order() << "|\n";
			ask_itr.next();
		} else {
			std::cout << OrderBook::EMPTY_ASK_ORDER << "|\n";
		}
	}

	ostream << "+-----------------------------------------------------------------+";

	return ostream;
}
