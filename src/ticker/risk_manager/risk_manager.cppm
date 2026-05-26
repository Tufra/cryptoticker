export module ticker.risk_manager;
import ticker.types;
import ticker.strategy.types;
import ticker.risk_manager.types;

export namespace ticker::risk_manager {
    class AbstractRiskManager {
        public:
            virtual ticker::risk_manager::types::RiskManagerDecision decide(
                const ticker::strategy::types::StrategyContext &context,
                const ticker::strategy::types::StrategyDecision &decision
            ) const = 0;

            virtual ~AbstractRiskManager() = default;
    };

    class SimpleRiskManager : public AbstractRiskManager {
        private:
            double max_share;
            double stop_loss;
            double take_profit;

            double adjust_volume(const double cash, const double price) const {
                const auto max_cash = cash * max_share;
                return max_cash / price;
            }

        public:
            SimpleRiskManager(
                const double max_share,
                const double stop_loss,
                const double take_profit
            )
                : max_share(max_share), stop_loss(stop_loss),
                  take_profit(take_profit) {}

            ticker::risk_manager::types::RiskManagerDecision decide(
                const ticker::strategy::types::StrategyContext &context,
                const ticker::strategy::types::StrategyDecision &decision
            ) const override {
                const auto price = decision.price;
                auto volume = decision.volume;
                auto total_spent = price * volume;
                const auto cash = context.portfolio_state.get_cash();
                const auto share = total_spent / cash;

                if (decision.signal == ticker::types::TradeSignal::Buy) {

                    if (share > max_share) {
                        volume = adjust_volume(cash, price);
                        total_spent = price * volume;
                    }

                    return ticker::risk_manager::types::RiskManagerDecision{
                        .timestamp = decision.timestamp,
                        .type = volume != decision.volume
                                    ? ticker::risk_manager::types::
                                          RiskManagerDecisionType::Override
                                    : ticker::risk_manager::types::
                                          RiskManagerDecisionType::Allow,

                        .signal = decision.signal,
                        .volume = volume,
                        .price = price,
                        .stop_loss = stop_loss,
                        .take_profit = take_profit
                    };
                } else {
                    return ticker::risk_manager::types::RiskManagerDecision{
                        .timestamp = decision.timestamp,
                        .type = ticker::risk_manager::types::
                            RiskManagerDecisionType::Allow,
                        .signal = decision.signal,
                        .volume = volume,
                        .price = price,
                        .stop_loss = stop_loss,
                        .take_profit = take_profit
                    };
                }
            }
    };
} // namespace ticker::risk_manager
