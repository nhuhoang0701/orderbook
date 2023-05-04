#pragma once

#include "order.hpp"
#include <cstdint>

class IcebergOrder : public Order {
  public:
	IcebergOrder(char type, int uid, int16_t price, int quantity, int peak_size);

	auto get_public_quantity() const -> int override;
	auto update_public_quantity() -> void override;

  private:
	int m_orig_quantity;
	int m_public_peak_size;
	int m_peak_size;
};
