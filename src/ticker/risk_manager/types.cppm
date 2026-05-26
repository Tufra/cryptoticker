module;

#include <optional>
#include <stdexcept>
#include <string>

export module ticker.risk_manager.types;
import ticker.types;

export namespace ticker::risk_manager::types {

    enum class RiskManagerDecisionType { Allow, Override, Reject };

    constexpr std::string
    risk_manager_decision_type_to_str(const RiskManagerDecisionType source) {
        if (source == RiskManagerDecisionType::Allow) {
            return "allow";
        } else if (source == RiskManagerDecisionType::Override) {
            return "override";
        } else if (source == RiskManagerDecisionType::Reject) {
            return "reject";
        } else {
            throw std::runtime_error("Unknown data source");
        }
    };

    struct RiskManagerDecision {
        public:
            long long timestamp;
            RiskManagerDecisionType type;
            ticker::types::TradeSignal signal;
            double volume;
            double price;
            std::optional<double> stop_loss;
            std::optional<double> take_profit;

            ticker::types::TradeDecision to_trade_decision() const {
                return ticker::types::TradeDecision{
                    .timestamp = timestamp,
                    .signal = signal,
                    .price = price,
                    .volume = volume,
                    .stop_loss = stop_loss,
                    .take_profit = take_profit
                };
            }
    };
} // namespace ticker::risk_manager::types
