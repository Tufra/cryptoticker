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

            double get_cash() const {
                return cash;
            }

            double get_position() const {
                return position;
            }

            void add_cash(double cash) {
                this->cash += cash;
            }

            void add_position(double position) {
                this->position += position;
            }

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
    };

} // namespace ticker::types
