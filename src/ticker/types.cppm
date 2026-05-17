module;
#include <optional>
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
