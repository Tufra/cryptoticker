module;

#include <filesystem>
#include <ranges>
#include <vector>

export module ticker.input;
import ticker.binance.types;
import ticker.types;

import ticker.binance.input;
import ticker.bybit.input;

export namespace ticker::input {
    std::vector<ticker::types::PriceTick> read_market_data(
        const ticker::types::DataSource source,
        std::filesystem::path input_path
    ) {
        std::vector<ticker::types::PriceTick> price_ticks = {};

        if (source == ticker::types::DataSource::Binance) {
            auto entries = ticker::binance::input::read_price_data(input_path);
            auto transformed_price_ticks =
                entries | std::views::transform(
                              ticker::binance::input::data_entry_to_tick
                          );
            for (const auto &entry : transformed_price_ticks) {
                price_ticks.push_back(entry);
            }
        } else if (source == ticker::types::DataSource::Bybit) {
            auto entries = ticker::bybit::input::read_price_data(input_path);
            auto transformed_price_ticks =
                entries |
                std::views::transform(ticker::bybit::input::data_entry_to_tick);
            for (const auto &entry : transformed_price_ticks) {
                price_ticks.push_back(entry);
            }
        } else {
            throw std::runtime_error(
                "Unknown data source: " +
                ticker::types::data_source_to_str(source)
            );
        }

        return price_ticks;
    }
} // namespace ticker::input
