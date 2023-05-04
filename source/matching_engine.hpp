#pragma once

#include "order.hpp"
#include "trade.hpp"
#include <iostream>
#include <memory>

class OrderBook;
class MatchingEngine {
  public:
	explicit MatchingEngine(OrderBook& order_book);

	auto match(const std::shared_ptr<Order>& order) -> void;

  private:
	OrderBook& m_order_book;
};
