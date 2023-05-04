#include "iceberg_order.hpp"
#include "limit_order.hpp"
#include "macros.hpp"
#include "order_book.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>

void test_aggressive_iceberg_order() {
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	auto book = OrderBook{};

	book.add_order(std::make_shared<IcebergOrder>('B', 1, 99, 50'000, 50));
	book.add_order(std::make_shared<IcebergOrder>('B', 2, 98, 25'500, 25));
	book.add_order(std::make_shared<IcebergOrder>('S', 3, 100, 10'000, 25));
	book.add_order(std::make_shared<IcebergOrder>('S', 4, 100, 7'500, 25));
	book.add_order(std::make_shared<IcebergOrder>('S', 5, 101, 20'000, 25));

	// Fill sell orders with an iceberg
	book.add_order(std::make_shared<IcebergOrder>('B', 6, 100, 100'000, 10'000));
	const auto& trades = utils::split_str(buffer.str(), '\n');
	ASSERT_EQUAL(trades.size(), 2);

	// Matching first B order
	ASSERT_EQUAL(book.get_bid_tree().size(), 3);
	ASSERT_EQUAL(book.get_ask_tree().size(), 1);
	ASSERT_EQUAL(trades[0], "6,3,100,10000");
	ASSERT_EQUAL(trades[1], "6,4,100,7500");
	ASSERT_EQUAL(book.get_bid_tree()[100].back()->get_public_quantity(), 10'000);
	ASSERT_EQUAL(book.get_bid_tree()[100].back()->get_quantity(), 82'500);

	std::cout.rdbuf(sbuf);
}

void test_passive_iceberg_order() {
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	auto book = OrderBook{};

	book.add_order(std::make_shared<IcebergOrder>('B', 1, 100, 82'500, 10'000));
	book.add_order(std::make_shared<LimitOrder>('B', 2, 99, 50'000));
	book.add_order(std::make_shared<LimitOrder>('B', 3, 98, 25'500));
	book.add_order(std::make_shared<LimitOrder>('S', 4, 101, 20'000));
	book.add_order(std::make_shared<LimitOrder>('S', 5, 100, 11'000));

	const auto& trades = utils::split_str(buffer.str(), '\n');
	ASSERT_EQUAL(trades.size(), 1);

	// Matching first B order
	ASSERT_EQUAL(book.get_bid_tree().size(), 3);
	ASSERT_EQUAL(book.get_ask_tree().size(), 1);
	ASSERT_EQUAL(trades[0], "1,5,100,11000");
	ASSERT_EQUAL(book.get_bid_tree()[100].back()->get_public_quantity(), 9'000);
	ASSERT_EQUAL(book.get_bid_tree()[100].back()->get_quantity(), 71'500);

	std::cout.rdbuf(sbuf);
}

int main() {
	test_aggressive_iceberg_order();
}
