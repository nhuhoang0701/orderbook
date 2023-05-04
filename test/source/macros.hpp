// #pragma once

#include <cassert>

#define ASSERT_EQUAL(res, expected) \
	assert(res == expected)
	// (static_cast<bool>(res == expected) ? void(0) : [&]() {
	// 	std::ostringstream error;
	// 	__assert_fail(error.str().c_str(), __FILE__, __LINE__, __PRETTY_FUNCTION__);
	// }())
