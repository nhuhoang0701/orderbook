// #include "matching_engine.hpp"
#include "iceberg_order.hpp"
#include "limit_order.hpp"
#include "macros.hpp"
#include "order_book.hpp"
#include "utils.hpp"

#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

auto test_normal_order() -> std::vector<std::tuple<std::string, std::string, std::string>> {
	return {std::make_tuple("NormalOrder",
							"B,1,1,1\n"
							"S,2,2,1",
							"+-----------------------------------------------------------------+\n"
							"| BUY                            | SELL                           |\n"
							"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
							"+----------+-------------+-------+-------+-------------+----------+\n"
							"|         1|            1|      1|       |             |          |\n"
							"+-----------------------------------------------------------------+\n"
							"+-----------------------------------------------------------------+\n"
							"| BUY                            | SELL                           |\n"
							"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
							"+----------+-------------+-------+-------+-------------+----------+\n"
							"|         1|            1|      1|      2|            1|         2|\n"
							"+-----------------------------------------------------------------+")};
}

auto test_order_with_comment() -> std::vector<std::tuple<std::string, std::string, std::string>> {
	return {std::make_tuple("OrderWithComment",
							"\n"
							"# Comment\n"
							" # Another valid comment\n"
							"B,1,1,1\n"
							"S,2,2,1",
							"+-----------------------------------------------------------------+\n"
							"| BUY                            | SELL                           |\n"
							"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
							"+----------+-------------+-------+-------+-------------+----------+\n"
							"|         1|            1|      1|       |             |          |\n"
							"+-----------------------------------------------------------------+\n"
							"+-----------------------------------------------------------------+\n"
							"| BUY                            | SELL                           |\n"
							"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
							"+----------+-------------+-------+-------+-------------+----------+\n"
							"|         1|            1|      1|      2|            1|         2|\n"
							"+-----------------------------------------------------------------+")};
}

auto test_order_format() -> std::vector<std::tuple<std::string, std::string, std::string>> {
	return {
		std::make_tuple("OrderIdFormat",
						"B,123456789,1,1\n"
						"S,123456780,2,1",
						"+-----------------------------------------------------------------+\n"
						"| BUY                            | SELL                           |\n"
						"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						"+----------+-------------+-------+-------+-------------+----------+\n"
						"| 123456789|            1|      1|       |             |          |\n"
						"+-----------------------------------------------------------------+\n"
						"+-----------------------------------------------------------------+\n"
						"| BUY                            | SELL                           |\n"
						"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						"+----------+-------------+-------+-------+-------------+----------+\n"
						"| 123456789|            1|      1|      2|            1| 123456780|\n"
						"+-----------------------------------------------------------------+"),
		std::make_tuple("OrderPriceFormat",
						"B,1,12345,1\n"
						"S,2,12346,1",
						"+-----------------------------------------------------------------+\n"
						"| BUY                            | SELL                           |\n"
						"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						"+----------+-------------+-------+-------+-------------+----------+\n"
						"|         1|            1| 12,345|       |             |          |\n"
						"+-----------------------------------------------------------------+\n"
						"+-----------------------------------------------------------------+\n"
						"| BUY                            | SELL                           |\n"
						"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						"+----------+-------------+-------+-------+-------------+----------+\n"
						"|         1|            1| 12,345| 12,346|            1|         2|\n"
						"+-----------------------------------------------------------------+"),
		std::make_tuple("OrderVolumeFormat",
						"S,1,2,1234567890\n"
						"B,2,1,1234567890",
						"+-----------------------------------------------------------------+\n"
						"| BUY                            | SELL                           |\n"
						"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						"+----------+-------------+-------+-------+-------------+----------+\n"
						"|          |             |       |      2|1,234,567,890|         1|\n"
						"+-----------------------------------------------------------------+\n"
						"+-----------------------------------------------------------------+\n"
						"| BUY                            | SELL                           |\n"
						"| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						"+----------+-------------+-------+-------+-------------+----------+\n"
						"|         2|1,234,567,890|      1|      2|1,234,567,890|         1|\n"
						"+-----------------------------------------------------------------+"),
	};
}

auto test_single_trade() -> std::vector<std::tuple<std::string, std::string, std::string>> {
	return {
		std::make_tuple<>("SingleTrade",
						  ""
						  "B,1,1,2\n"
						  "S,2,2,1\n"
						  "S,3,1,1",
						  "+-----------------------------------------------------------------+\n"
						  "| BUY                            | SELL                           |\n"
						  "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						  "+----------+-------------+-------+-------+-------------+----------+\n"
						  "|         1|            2|      1|       |             |          |\n"
						  "+-----------------------------------------------------------------+\n"
						  "+-----------------------------------------------------------------+\n"
						  "| BUY                            | SELL                           |\n"
						  "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						  "+----------+-------------+-------+-------+-------------+----------+\n"
						  "|         1|            2|      1|      2|            1|         2|\n"
						  "+-----------------------------------------------------------------+\n"
						  "1,3,1,1\n"
						  "+-----------------------------------------------------------------+\n"
						  "| BUY                            | SELL                           |\n"
						  "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
						  "+----------+-------------+-------+-------+-------------+----------+\n"
						  "|         1|            1|      1|      2|            1|         2|\n"
						  "+-----------------------------------------------------------------+")};
}

auto get_order_book_output(const std::string& input) -> std::string {
	std::istringstream istream(input);
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());

	auto order_book = OrderBook{};
	auto line = std::string{};
	const auto comment_regex = std::regex{"^\\s*#"};
	while (std::getline(istream, line)) {
		// Skip if empty line or comment
		if (line.empty() || std::regex_search(line, comment_regex)) {
			continue;
		}

		auto tokens = utils::split_str(line, ',');
		auto type = tokens[0][0];
		auto uid = std::stoi(tokens[1]);
		auto price = static_cast<int16_t>(std::stoi(tokens[2]));
		auto quantity = std::stoi(tokens[3]);

		// Matching orders
		if (tokens.size() == 4) { // limit order (4 tokens)
			auto limit_order = std::make_shared<LimitOrder>(type, uid, price, quantity);
			order_book.add_order(limit_order);

		} else { // iceberg order (5 tokens)
			auto peak_size = std::stoi(tokens[4]);
			auto iceberg_order =
				std::make_shared<IcebergOrder>(type, uid, price, quantity, peak_size);
			order_book.add_order(iceberg_order);
		}

		// Display current order book
		std::cout << order_book << '\n';
	}

	auto output = buffer.str();
	std::cout.rdbuf(sbuf);

	// Remove last '\n' in output
	return output.substr(0, output.size() - 1);
}

void run_test(const std::string& input, const std::string& expected) {
	const auto std_out = get_order_book_output(input);
	std::cout << std_out;

	ASSERT_EQUAL(expected, std_out);
}

auto main() -> int {
	auto tests = {test_normal_order, test_order_with_comment, test_order_format, test_single_trade};
	for (const auto& test : tests) {
		const auto& results = test();
		for (const auto& res : results) {
			const auto& input = std::get<1>(res);
			const auto& expected_output = std::get<2>(res);
			run_test(input, expected_output);
		}
	}
}
