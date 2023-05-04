#include "matching_engine.hpp"
#include "order_book.hpp"
#include "trade.hpp"

#include <iostream>
#include <memory>
MatchingEngine::MatchingEngine(OrderBook& order_book) : m_order_book{order_book} {
}

template <typename T>
auto try_match(T& limit_tree, const std::shared_ptr<Order>& order) -> void {
	while (!limit_tree.empty() && order->get_quantity() != 0) {
		auto tree_itr = limit_tree.begin();
		auto top_price = tree_itr->first;
		auto is_matchable = (order->is_bid() ? (top_price <= order->get_price())
											 : (top_price >= order->get_price()));
		if (!is_matchable) {
			break;
		}

		auto& orders = tree_itr->second;
		auto& top_order = orders.front();
		auto matched_quantity = std::min(top_order->get_quantity(), order->get_quantity());

		auto bid_uid = top_order->get_uid();
		auto ask_uid = order->get_uid();
		if (order->is_bid()) {
			std::swap(bid_uid, ask_uid);
		}

		// Print trade
		std::cout << Trade{bid_uid, ask_uid, top_price, matched_quantity} << '\n';

		// Update quantity of current order
		order->set_quantity(order->get_quantity() - matched_quantity);

		// Update quantity of top_order, remove it if updated quantity is 0
		if (matched_quantity == top_order->get_quantity()) {
			orders.pop_front();
		} else {
			top_order->set_quantity(top_order->get_quantity() - matched_quantity);
		}

		// Erase current Limit node if list of order is empty
		if (orders.empty()) {
			limit_tree.erase(limit_tree.begin());
		}

		// Update public quantity in case passive iceberg order is matched
		top_order->update_public_quantity();
	}

	// Update public quantity in case aggressive iceberg order is matched
	order->update_public_quantity();
}

auto MatchingEngine::match(const std::shared_ptr<Order>& order) -> void {
	if (order->is_bid()) {
		try_match(m_order_book.get_ask_tree(), order);
	} else {
		try_match(m_order_book.get_bid_tree(), order);
	}
}
