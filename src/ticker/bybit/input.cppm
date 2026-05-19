module;

#include <filesystem>
#include <fstream>
#include <string>

#include <boost/json.hpp>
#include <boost/json/value.hpp>

export module ticker.bybit.input;
import ticker.bybit.types;
import ticker.types;

export namespace ticker::bybit::input {

    std::vector<ticker::bybit::types::BybitKlineDataEntry>
    read_price_data(const std::filesystem::path &filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error(
                "Failed to open file: " + filename.string()
            );
        }

        std::vector<ticker::bybit::types::BybitKlineDataEntry>
            price_data_entries;
        std::string line;
        std::string buffer;

        while (std::getline(file, line)) {
            buffer += line;
        }

        boost::json::value parsed_data = boost::json::parse(buffer);
        if (!parsed_data.is_object()) {
            throw std::runtime_error(
                "Expected JSON object in file: " + filename.string()
            );
        }
        auto parsed_obj = parsed_data.as_object();

        if (!(parsed_obj.contains("retMsg") &&
              parsed_obj["retMsg"].is_string() &&
              parsed_obj["retMsg"].as_string() == "OK")) {
            throw std::runtime_error(
                "Expected 'retMsg' key in JSON object in file to be 'OK': " +
                filename.string()
            );
        }
        if (!(parsed_obj.contains("result") &&
              parsed_obj["result"].is_object() &&
              parsed_obj["result"].as_object().contains("list") &&
              parsed_obj["result"].as_object()["list"].is_array())) {
            throw std::runtime_error(
                "Expected 'result' key in JSON object in file to contain "
                "'list' key which is an array: " +
                filename.string()
            );
        }

        boost::json::array kline_data =
            parsed_obj["result"].as_object()["list"].as_array();

        for (auto i = kline_data.rbegin(); i != kline_data.rend(); ++i) {
            auto entry = *i;

            if (!entry.is_array()) {
                throw std::runtime_error(
                    "Expected JSON arrays in file: " + filename.string()
                );
            }

            const auto &data = entry.as_array();
            if (data.size() != 7) {
                throw std::runtime_error(
                    "Expected 7 elements in each price data entry in "
                    "file: " +
                    filename.string()
                );
            }

            long long open_time = std::stoll(data[0].as_string().c_str());

            double open_price = std::stod(data[1].as_string().c_str());
            double close_price = std::stod(data[4].as_string().c_str());
            double high_price = std::stod(data[2].as_string().c_str());
            double low_price = std::stod(data[3].as_string().c_str());
            double volume = std::stod(data[5].as_string().c_str());

            double turnover = std::stod(data[6].as_string().c_str());

            ticker::bybit::types::BybitKlineDataEntry price_data_entry{
                open_time,   open_price, high_price, low_price,
                close_price, volume,     turnover
            };

            price_data_entries.push_back(price_data_entry);
        }

        return price_data_entries;
    }

    ticker::types::PriceTick
    data_entry_to_tick(const ticker::bybit::types::BybitKlineDataEntry &entry) {
        return ticker::types::PriceTick{entry.close_price, entry.open_time};
    }

} // namespace ticker::bybit::input
