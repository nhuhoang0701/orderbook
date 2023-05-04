#pragma once
#include <sstream>
#include <string>
#include <vector>

namespace utils {
	auto split_str(const std::string& input, char delimiter = ' ') -> std::vector<std::string>;
	auto add_commas(int n) -> std::string;
} // namespace utils
