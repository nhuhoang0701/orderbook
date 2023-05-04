#pragma once

#include <cstdint>
#include <ostream>

class Trade {
  public:
	Trade(int buy_id, int sell_id, int16_t price, int quantity);
	friend auto operator<<(std::ostream& ostream, const Trade& trade) -> std::ostream&;

  private:
	int m_buy_id;
	int m_sell_id;
	int16_t m_price;
	int m_quantity;
};
