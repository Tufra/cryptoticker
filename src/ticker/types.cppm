module;
#include <optional>
#include <stdexcept>
#include <string_view>

export module ticker.types;

export namespace ticker::types {

    enum class TradeSignal { Buy, Sell, Hold };

    constexpr std::string_view signal_to_string(const TradeSignal signal) {
        switch (signal) {
            case TradeSignal::Buy: {
                return "Buy";
            }
            case TradeSignal::Sell: {
                return "Sell";
            }
            case TradeSignal::Hold: {
                return "Hold";
            }
        }

        return "Unknown";
    };

    enum class DataSource { Binance, Bybit };

    constexpr DataSource str_to_data_source(const std::string_view str) {
        if (str == "binance") {
            return DataSource::Binance;
        } else if (str == "bybit") {
            return DataSource::Bybit;
        } else {
            throw std::runtime_error(
                "Unknown data source: " + std::string(str)
            );
        }
    };

    constexpr std::string data_source_to_str(const DataSource source) {
        if (source == DataSource::Binance) {
            return "binance";
        } else if (source == DataSource::Bybit) {
            return "bybit";
        } else {
            throw std::runtime_error("Unknown data source");
        }
    };

    struct PriceTick {
        public:
            double price;
            long long timestamp;

            bool operator==(const PriceTick &other) const {
                return price == other.price && timestamp == other.timestamp;
            }
    };

    struct StatsRow {
        public:
            long long timestamp;
            double price;
            std::optional<double> sma;
            std::optional<double> volatility;
            std::optional<TradeSignal> signal;
    };

} // namespace ticker::types
