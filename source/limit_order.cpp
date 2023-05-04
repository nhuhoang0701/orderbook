
#include "limit_order.hpp"
#include "order.hpp"
#include "utils.hpp"
#include <cstdint>
#include <iomanip>

LimitOrder::LimitOrder(char type, int uid, int16_t price, int quantity)
	: Order{type, uid, price, quantity} {
}
