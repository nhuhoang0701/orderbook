#include "limit_order.hpp"
#include "macros.hpp"
#include "order_book.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>

void test_limit_order_simple() {
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	auto book = OrderBook{};

	book.add_order(std::make_shared<LimitOrder>('B', 1, 101, 10));
	book.add_order(std::make_shared<LimitOrder>('B', 2, 101, 10));

	book.add_order(std::make_shared<LimitOrder>('S', 3, 103, 10));

	ASSERT_EQUAL(buffer.str().empty(), true);

	// Matching first B order
	book.add_order(std::make_shared<LimitOrder>('S', 4, 101, 10));
	const auto& trades = utils::split_str(buffer.str(), '\n');
	ASSERT_EQUAL(trades.size(), 1);
	ASSERT_EQUAL(book.get_bid_tree().size(), 1);
	ASSERT_EQUAL(book.get_ask_tree().size(), 1);
	ASSERT_EQUAL(trades[0], "1,4,101,10");

	std::cout.rdbuf(sbuf);
}

void test_limit_order_advanced() {
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	auto book = OrderBook{};

	book.add_order(std::make_shared<LimitOrder>('B', 1, 101, 10));
	book.add_order(std::make_shared<LimitOrder>('B', 2, 101, 10));

	book.add_order(std::make_shared<LimitOrder>('S', 3, 103, 10));
	ASSERT_EQUAL(buffer.str().empty(), true);

	// Fill both B orders
	book.add_order(std::make_shared<LimitOrder>('S', 5, 101, 20));
	const auto& trades = utils::split_str(buffer.str(), '\n');
	ASSERT_EQUAL(trades.size(), 2);
	ASSERT_EQUAL(book.get_bid_tree().size(), 0);
	ASSERT_EQUAL(book.get_ask_tree().size(), 1);

	ASSERT_EQUAL(trades[0], "1,5,101,10");
	ASSERT_EQUAL(trades[1], "2,5,101,10");

	std::cout.rdbuf(sbuf);
}

void test_limit_order_partial() {
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	auto book = OrderBook{};

	book.add_order(std::make_shared<LimitOrder>('B', 1, 101, 10));
	book.add_order(std::make_shared<LimitOrder>('B', 2, 101, 10));

	// Fill both B orders
	book.add_order(std::make_shared<LimitOrder>('S', 5, 101, 15));
	const auto& trades = utils::split_str(buffer.str(), '\n');
	ASSERT_EQUAL(trades.size(), 2);

	ASSERT_EQUAL(book.get_bid_tree().size(), 1);
	ASSERT_EQUAL(book.get_ask_tree().size(), 0);

	ASSERT_EQUAL(trades[0], "1,5,101,10");
	ASSERT_EQUAL(trades[1], "2,5,101,5");

	std::cout.rdbuf(sbuf);
}

auto main() -> int {
	test_limit_order_simple();
	test_limit_order_advanced();
	test_limit_order_partial();
}
