#include "iceberg_order.hpp"
#include "limit_order.hpp"
#include "order_book.hpp"
#include "utils.hpp"

#include <iostream>
#include <istream>
#include <memory>
#include <regex>
#include <string>

auto main() -> int {
	auto order_book = OrderBook{};
	auto line = std::string{};
	const auto comment_regex = std::regex{"^\\s*#"};

	while (std::getline(std::cin, line)) {
		// Skip if empty line or comment
		if (line.empty() || std::regex_search(line, comment_regex)) {
			continue;
		}

		// Extract info
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

	return 0;
}
