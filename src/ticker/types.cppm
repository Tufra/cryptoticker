module;
#include <optional>
#include <stdexcept>
#include <string_view>

export module ticker.types;

export namespace ticker::types {

    enum class TradeSignal { Buy, Sell, Hold };

    constexpr std::string signal_to_str(const TradeSignal signal) {
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

    enum class Mode { Backtest, Ticks };

    constexpr Mode str_to_mode(const std::string_view str) {
        if (str == "backtest") {
            return Mode::Backtest;
        } else if (str == "ticks") {
            return Mode::Ticks;
        } else {
            throw std::runtime_error("Unknown mode: " + std::string(str));
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

    struct PortfolioState {
        private:
            double cash;
            double position;

        public:
            PortfolioState(double cash, double position = 0)
                : cash(cash), position(position) {}

            double get_cash() const { return cash; }

            double get_position() const { return position; }

            void add_cash(double cash) { this->cash += cash; }

            void add_position(double position) { this->position += position; }

            void remove_cash(double cash) {
                if (cash > this->cash) {
                    throw std::runtime_error("Not enough cash");
                }
                this->cash -= cash;
            }

            void remove_position(double position) {
                if (position > this->position) {
                    throw std::runtime_error("Not enough position");
                }
                this->position -= position;
            }

            double get_equity(double price) const {
                return cash + position * price;
            }
    };

    struct Trade {
        public:
            long long timestamp;
            ticker::types::TradeSignal signal;
            double price;
            double volume;
            double cash_after;
            double position_after;
            double commission;

            bool success;

            std::string to_string() const {
                return "Timestamp: " + std::to_string(timestamp) +
                       "\nSignal: " + ticker::types::signal_to_str(signal) +
                       "\nPrice: " + std::to_string(price) +
                       "\nQuantity: " + std::to_string(volume) +
                       "\nCash After: " + std::to_string(cash_after) +
                       "\nPosition After: " + std::to_string(position_after) +
                       "\nCommission: " + std::to_string(commission) +
                       "\nSuccess: " + std::to_string(success) + "\n\n";
            };
    };

    struct TradeDecision {
        public:
            long long timestamp;
            TradeSignal signal;
            double price;
            double volume;
            std::optional<double> stop_loss;
            std::optional<double> take_profit;

            std::string to_string() const {
                return "Timestamp: " + std::to_string(timestamp) +
                       "\nSignal: " + ticker::types::signal_to_str(signal) +
                       "\nPrice: " + std::to_string(price) +
                       "\nQuantity: " + std::to_string(volume) +
                       "\nStop Loss: " + std::to_string(stop_loss.value_or(0)) +
                       "\nTake Profit: " +
                       std::to_string(take_profit.value_or(0)) + "\n\n";
            };
    };

} // namespace ticker::types
