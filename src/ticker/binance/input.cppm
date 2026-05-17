module;

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <boost/json.hpp>
#include <boost/json/value.hpp>

export module ticker.binance.input;
import ticker.binance.types;
import ticker.types;

export namespace ticker::binance::input {

    std::vector<ticker::binance::types::BinanceKlineDataEntry>
    read_binance_price_data(const std::filesystem::path &filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error(
                "Failed to open file: " + filename.string()
            );
        }

        std::vector<ticker::binance::types::BinanceKlineDataEntry>
            price_data_entries;
        std::string line;
        std::string buffer;

        while (std::getline(file, line)) {
            buffer += line;
        }

        boost::json::value parsed_data = boost::json::parse(buffer);
        if (!parsed_data.is_array()) {
            throw std::runtime_error(
                "Expected JSON array in file: " + filename.string()
            );
        }

        for (const auto &entry : parsed_data.as_array()) {
            if (!entry.is_array()) {
                throw std::runtime_error(
                    "Expected JSON arrays in file: " + filename.string()
                );
            }

            const auto &data = entry.as_array();
            if (data.size() != 12) {
                throw std::runtime_error(
                    "Expected 12 elements in each price data entry in file: " +
                    filename.string()
                );
            }

            long long open_time = data[0].as_int64();
            long long close_time = data[6].as_int64();
            long long number_of_trades = data[8].as_int64();

            double open_price = std::stod(data[1].as_string().c_str());
            double close_price = std::stod(data[4].as_string().c_str());
            double high_price = std::stod(data[2].as_string().c_str());
            double low_price = std::stod(data[3].as_string().c_str());
            double volume = std::stod(data[5].as_string().c_str());

            double quote_asset_volume = std::stod(data[7].as_string().c_str());
            double taker_buy_base_asset_volume =
                std::stod(data[9].as_string().c_str());
            double taker_buy_quote_asset_volume =
                std::stod(data[10].as_string().c_str());

            ticker::binance::types::BinanceKlineDataEntry price_data_entry{
                open_time,
                open_price,
                high_price,
                low_price,
                close_price,
                volume,
                close_time,
                quote_asset_volume,
                number_of_trades,
                taker_buy_base_asset_volume,
                taker_buy_quote_asset_volume,
            };

            price_data_entries.push_back(price_data_entry);
        }

        return price_data_entries;
    }

    ticker::types::PriceTick data_entry_to_tick(
        const ticker::binance::types::BinanceKlineDataEntry &entry
    ) {
        return ticker::types::PriceTick{entry.close_price, entry.close_time};
    }

} // namespace ticker::binance::input
