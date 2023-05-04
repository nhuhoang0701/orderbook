#pragma once

#include "matching_engine.hpp"
// #include "matching_engine.hpp"
#include <functional>
#include <istream>
#include <iterator>
#include <list>
#include <map>
#include <memory>

class MatchingEngine;
class OrderBook {
  public:
	using limit_t = int16_t;
	using bid_tree_t = std::map<limit_t, std::list<std::shared_ptr<Order>>, std::greater<>>;
	using ask_tree_t = std::map<limit_t, std::list<std::shared_ptr<Order>>>;

	static const std::string EMPTY_BID_ORDER;
	static const std::string EMPTY_ASK_ORDER;

	OrderBook();

	auto add_order(const std::shared_ptr<Order>& order) -> void;
	auto get_bid_tree() -> bid_tree_t&;
	auto get_ask_tree() -> ask_tree_t&;

	friend auto operator<<(std::ostream& ostream, const OrderBook& order_book) -> std::ostream&;

  private:
	MatchingEngine m_matching_engine;
	bid_tree_t m_bids;
	ask_tree_t m_asks;
};

template <typename T>
class OrderIterator {
  public:
	explicit OrderIterator(const T& limit_tree) : m_limit_tree{limit_tree} {
		if (!m_limit_tree.empty()) {
			m_is_done = false;
			m_limit = m_limit_tree.begin();
			m_orders = m_limit->second;
			m_cur_order = m_orders.begin();
		}
	}

	auto is_done() const -> bool {
		return m_is_done;
	}

	auto next() -> void {
		std::advance(m_cur_order, 1);

		if (m_cur_order == m_orders.end()) {
			std::advance(m_limit, 1);

			if (m_limit == m_limit_tree.end()) {
				m_is_done = true;
				return;
			}
			m_orders = m_limit->second;
			m_cur_order = m_orders.begin();
		}
	}

	auto get_order() const -> std::shared_ptr<Order> {
		if (is_done()) {
			return nullptr;
		}

		return *m_cur_order;
	}

  private:
	bool m_is_done{true};
	const T& m_limit_tree;
	typename T::const_iterator m_limit{};
	std::list<std::shared_ptr<Order>> m_orders{};
	std::list<std::shared_ptr<Order>>::const_iterator m_cur_order{};
};
