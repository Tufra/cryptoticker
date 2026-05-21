module;

#include <span>

export module ticker.strategy.types;
import ticker.types;

export namespace ticker::strategy::types {
    struct StrategyDecision {
        public:
            ticker::types::TradeSignal signal;
            double volume;
    };

    struct StrategyContext {
        public:
            std::span<const ticker::types::StatsRow> history;
            const ticker::types::PortfolioState &portfolio_state;
    };
} // namespace ticker::strategy::types
