#include "order.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>


Order::Order(char type, int uid, int16_t price, int quantity)
	: m_type{type}, m_uid{uid}, m_price{price}, m_quantity{quantity} {
}

auto Order::is_bid() const -> bool {
	return m_type == 'B';
}

auto Order::get_uid() const -> int {
	return m_uid;
}

auto Order::get_price() const -> int16_t {
	return m_price;
}

auto Order::get_quantity() const -> int {
	return m_quantity;
}
auto Order::get_public_quantity() const -> int {
	return m_quantity;
}

auto Order::update_public_quantity() -> void {
	// Do nothing by default
}

auto Order::set_quantity(int quantity) -> void {
	m_quantity = quantity;
}

auto operator<<(std::ostream& ostream, const Order& order) -> std::ostream& {
	// Work-around to get polymorphism of operator<<
	order.print(ostream);
	return ostream;
}

auto Order::print(std::ostream& ostream) const -> void {
	if (is_bid()) {
		ostream << std::right << std::setw(Order::ID_WIDTH) << get_uid() << "|"
				<< std::setw(Order::VOL_WIDTH) << utils::add_commas(get_public_quantity()) << "|"
				<< std::setw(Order::PRICE_WIDTH) << utils::add_commas(get_price());
	} else {
		ostream << std::right << std::setw(Order::PRICE_WIDTH) << utils::add_commas(get_price())
				<< "|" << std::setw(Order::VOL_WIDTH) << utils::add_commas(get_public_quantity())
				<< "|" << std::setw(Order::ID_WIDTH) << get_uid();
	}
}
