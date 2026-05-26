module;

#include <span>
#include <optional>

export module ticker.strategy.types;
import ticker.types;

export namespace ticker::strategy::types {
    struct StrategyDecision {
        public:
            long long timestamp;
            ticker::types::TradeSignal signal;
            double volume;
            double price;

            ticker::types::TradeDecision to_trade_decision() const {
                return ticker::types::TradeDecision{
                    .timestamp = timestamp,
                    .signal = signal,
                    .price = price,
                    .volume = volume,
                    .stop_loss = std::nullopt,
                    .take_profit = std::nullopt
                };
            }
    };

    struct StrategyContext {
        public:
            std::span<const ticker::types::StatsRow> history;
            const ticker::types::PortfolioState &portfolio_state;
    };
} // namespace ticker::strategy::types
