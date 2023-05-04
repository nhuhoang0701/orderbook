#pragma once

#include <cstdint>
#include <ostream>

class Order {
  public:
	static constexpr int ID_WIDTH = 10;
	static constexpr int VOL_WIDTH = 13;
	static constexpr int PRICE_WIDTH = 7;

	Order(char type, int uid, int16_t price, int quantity);
	Order(const Order&) = default;
	Order(Order&&) = delete;
	auto operator=(const Order&) -> Order& = default;
	auto operator=(Order&&) -> Order& = delete;
	virtual ~Order() = default;

	auto is_bid() const -> bool;
	auto get_uid() const -> int;
	auto get_price() const -> int16_t;
	auto get_quantity() const -> int;
	auto set_quantity(int quantity) -> void;

	virtual auto get_public_quantity() const -> int;
	virtual auto update_public_quantity() -> void;

	friend auto operator<<(std::ostream& ostream, const Order& order) -> std::ostream&;

  protected:
	virtual auto print(std::ostream& ostream) const -> void;

  private:
	char m_type;
	int m_uid;
	int16_t m_price;
	int m_quantity;
};
