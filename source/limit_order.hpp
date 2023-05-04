#pragma once

#include "order.hpp"
#include <cstdint>
#include <string>

class LimitOrder : public Order {
  public:
	LimitOrder(char type, int uid, int16_t price, int quantity);

  private:
};
