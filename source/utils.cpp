#include "utils.hpp"
namespace utils {
	auto split_str(const std::string& input, char delimiter) -> std::vector<std::string> {
		auto istream = std::istringstream{input};
		auto tokens = std::vector<std::string>{};
		auto token = std::string{};

		while (std::getline(istream, token, delimiter)) {
			tokens.push_back(token);
		}
		return tokens;
	}

	auto add_commas(int n) -> std::string {
		auto str = std::to_string(n);
		auto len = str.length();

		if (len <= 3) {
			return str;
		}

		auto result = std::string{};
		auto comma_pos = len % 3;

		if (comma_pos == 0) {
			comma_pos = 3;
		}

		for (auto i = size_t{0}; i < len; i++) {
			result += str[i];

			if (i == comma_pos - 1 && i != len - 1) {
				result += ',';
				comma_pos += 3;
			}
		}

		return result;
	}

} // namespace utils
