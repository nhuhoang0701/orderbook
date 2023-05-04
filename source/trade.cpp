#include "trade.hpp"

Trade::Trade(int buy_id, int sell_id, int16_t price, int quantity)
	: m_buy_id{buy_id}, m_sell_id{sell_id}, m_price{price}, m_quantity{quantity} {
}

auto operator<<(std::ostream& ostream, const Trade& trade) -> std::ostream& {
	ostream << trade.m_buy_id << "," << trade.m_sell_id << "," << trade.m_price << ","
			<< trade.m_quantity;
	return ostream;
}
