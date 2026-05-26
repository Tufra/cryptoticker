export module ticker.executor;
import ticker.types;

export namespace ticker::executor {

    class AbstractExecutor {
        public:
            virtual ticker::types::Trade
            execute(const ticker::types::TradeDecision &decision) = 0;
            virtual ~AbstractExecutor() = default;
    };

    class SimulationExecutor : public AbstractExecutor {
        private:
            ticker::types::PortfolioState &portfolio_state;

        public:
            SimulationExecutor(ticker::types::PortfolioState &portfolio_state)
                : portfolio_state(portfolio_state) {}

            ticker::types::Trade execute(
                const ticker::types::TradeDecision &decision
            ) override {
                auto trade = ticker::types::Trade{
                    .timestamp = decision.timestamp,
                    .signal = decision.signal,
                    .price = decision.price,
                    .volume = decision.volume,
                    .cash_after = portfolio_state.get_cash(),
                    .position_after = portfolio_state.get_position(),
                    .commission = 0,
                    .success = true,
                };

                if (decision.signal == ticker::types::TradeSignal::Buy) {
                    trade.volume = decision.volume;
                    portfolio_state.add_position(trade.volume);

                    double to_pay = decision.volume * decision.price;
                    portfolio_state.remove_cash(to_pay);

                } else if (
                    decision.signal == ticker::types::TradeSignal::Sell
                ) {
                    trade.volume = decision.volume;
                    portfolio_state.remove_position(trade.volume);

                    double to_receive = decision.volume * decision.price;
                    portfolio_state.add_cash(to_receive);

                } else {
                    trade.success = false;
                }

                return trade;
            }
    };
} // namespace ticker::executor
