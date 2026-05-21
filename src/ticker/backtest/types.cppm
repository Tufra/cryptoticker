module;

#include <cstddef>
#include <string>

export module ticker.backtest.types;
import ticker.types;

export namespace ticker::backtest::types {
    struct Trade {
        public:
            long long timestamp;
            ticker::types::TradeSignal signal;
            double price;
            double quantity;
            double cash_after;
            double position_after;
            double commission;

            std::string to_string() const {
                return "Timestamp: " + std::to_string(timestamp) +
                       "\nSignal: " + ticker::types::signal_to_str(signal) +
                       "\nPrice: " + std::to_string(price) +
                       "\nQuantity: " + std::to_string(quantity) +
                       "\nCash After: " + std::to_string(cash_after) +
                       "\nPosition After: " + std::to_string(position_after) +
                       "\n\n";
            };
    };

    struct BacktestReport {
        public:
            double starting_cash;
            double ending_equity;
            double pnl;
            std::size_t total_trades;
            double max_drawdown;

            std::string to_string() const {
                return "Starting Cash: " + std::to_string(starting_cash) +
                       "\nEnding Equity: " + std::to_string(ending_equity) +
                       "\nPnL: " + std::to_string(pnl) +
                       "\nTotal Trades: " + std::to_string(total_trades) +
                       "\nMax Drawdown: " + std::to_string(max_drawdown);
            }
    };
} // namespace ticker::backtest::types
