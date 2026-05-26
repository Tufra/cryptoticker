module;

#include <cstddef>
#include <string>

export module ticker.runner.types;
import ticker.types;

export namespace ticker::runner::types {
    struct RunnerReport {
        public:
            double starting_cash;
            double ending_equity;
            double pnl;
            std::size_t total_trades;
            double max_drawdown;

            std::string to_string() const {
                return "Starting Equity: " + std::to_string(starting_cash) +
                       "\nEnding Equity: " + std::to_string(ending_equity) +
                       "\nPnL: " + std::to_string(pnl) +
                       "\nTotal Trades: " + std::to_string(total_trades) +
                       "\nMax Drawdown: " + std::to_string(max_drawdown);
            }
    };
} // namespace ticker::backtest::types
