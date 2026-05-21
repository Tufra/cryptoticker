module;

#include <cmath>

export module ticker.strategy;
import ticker.types;
import ticker.strategy.types;

export namespace ticker::strategy {
    class AbstractStrategy {
        public:
            virtual ticker::strategy::types::StrategyDecision decide(
                const ticker::strategy::types::StrategyContext &context
            ) const = 0;

            virtual ~AbstractStrategy() = default;
    };

    class SimpleStrategy : public AbstractStrategy {
        private:
            double decide_volume_to_buy(double cash, double price) const {
                return std::floor(cash) / price;
            }

        public:
            ticker::strategy::types::StrategyDecision decide(
                const ticker::strategy::types::StrategyContext &context
            ) const override {
                const auto& row = context.history.back();
                const auto& portfolio_state = context.portfolio_state;

                if (!(row.sma.has_value() && row.volatility.has_value())) {
                    return ticker::strategy::types::StrategyDecision{
                        .signal = ticker::types::TradeSignal::Hold, .volume = 0
                    };
                }

                const auto price = row.price;
                const auto sma = row.sma.value();
                const auto volatility = row.volatility.value();

                if (price > sma + volatility) {
                    const double volume =
                        decide_volume_to_buy(portfolio_state.get_cash(), price);
                    return ticker::strategy::types::StrategyDecision{
                        .signal = ticker::types::TradeSignal::Buy,
                        .volume = volume
                    };
                } else if (price < sma - volatility) {
                    const double volume = portfolio_state.get_position();
                    return ticker::strategy::types::StrategyDecision{
                        .signal = ticker::types::TradeSignal::Sell,
                        .volume = volume
                    };
                } else {
                    return ticker::strategy::types::StrategyDecision{
                        .signal = ticker::types::TradeSignal::Hold, .volume = 0
                    };
                }
            }
    };
} // namespace ticker::strategy
