#include "macros.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

void test_split_str() {
	auto expected = std::vector<std::string>{"S", "100345", "5103", "10000", "2000"};
	ASSERT_EQUAL(utils::split_str("S,100345,5103,10000,2000", ','), expected);

	expected = std::vector<std::string>{"B", "100321", "5102", "9000"};
	ASSERT_EQUAL(utils::split_str("B,100321,5102,9000", ','), expected);
}

void test_add_commas() {
	auto input = std::vector<int>{1, 12, 123, 1234, 12345, 123456, 1234567, 12345678, 123456789};
	auto expected = std::vector<std::string>{
		"1", "12", "123", "1,234", "12,345", "123,456", "1,234,567", "12,345,678", "123,456,789"};

	for (auto i = size_t{0}; i < input.size(); i++) {
		ASSERT_EQUAL(utils::add_commas(input[i]), expected[i]);
	}
}

auto main() -> int {
	test_split_str();
	test_add_commas();
}
