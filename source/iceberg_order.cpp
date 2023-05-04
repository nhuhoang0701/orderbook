#include "iceberg_order.hpp"
#include "utils.hpp"
#include <iomanip>

IcebergOrder::IcebergOrder(char type, int uid, int16_t price, int quantity, int peak_size)
	: Order{type, uid, price, quantity}, m_orig_quantity{quantity}, m_public_peak_size{0},
	  m_peak_size{peak_size} {
}
auto IcebergOrder::get_public_quantity() const -> int {
	return m_public_peak_size;
}

auto IcebergOrder::update_public_quantity() -> void {
	// Aggressive iceberg order
	if (m_public_peak_size == 0) {
		m_public_peak_size = m_peak_size;
		return;
	}

	// Passive iceberg order
	m_public_peak_size = m_peak_size - (m_orig_quantity - get_quantity()) % m_peak_size;
}
