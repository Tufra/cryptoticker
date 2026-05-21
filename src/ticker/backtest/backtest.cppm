module;

#include <iostream>
#include <span>
#include <vector>

export module ticker.backtest;
import ticker.types;
import ticker.stats;
import ticker.strategy;
import ticker.strategy.types;
import ticker.backtest.types;

export namespace ticker::backtest {
    class BacktestEngine {
        private:
            const ticker::strategy::AbstractStrategy &strategy;
            std::vector<ticker::backtest::types::Trade> trade_log;
            ticker::types::PortfolioState &portfolio_state;

            std::size_t history_length;

        public:
            BacktestEngine(
                ticker::strategy::AbstractStrategy &strategy,
                ticker::types::PortfolioState &portfolio_state,
                std::size_t history_length = 10
            )
                : strategy(strategy), portfolio_state(portfolio_state),
                  history_length(history_length) {}

            const std::vector<ticker::backtest::types::Trade> &
            get_trade_log() const {
                return trade_log;
            }

            ticker::backtest::types::BacktestReport
            run_backtest(std::span<const ticker::types::StatsRow> stats) {
                auto report = ticker::backtest::types::BacktestReport{
                    .starting_cash = portfolio_state.get_cash(),
                    .ending_equity = portfolio_state.get_cash(),
                    .pnl = 0,
                    .total_trades = trade_log.size(),
                    .max_drawdown = 0
                };
                double last_price = 0;

                for (std::size_t i = 0; i < stats.size(); i++) {
                    const auto row = stats[i];

                    const auto history = ticker::stats::get_history_window(
                        stats, i, history_length
                    );

                    const ticker::strategy::types::StrategyDecision decision =
                        strategy.decide(
                            ticker::strategy::types::StrategyContext{
                                .history = history,
                                .portfolio_state = portfolio_state
                            }
                        );

                    auto trade = ticker::backtest::types::Trade{
                        .timestamp = row.timestamp,
                        .signal = decision.signal,
                        .price = row.price,
                        .quantity = decision.volume,
                        .cash_after = portfolio_state.get_cash(),
                        .position_after = portfolio_state.get_position(),
                        .commission = 0
                    };
                    last_price = row.price;

                    if (decision.signal == ticker::types::TradeSignal::Buy &&
                        decision.volume > 0) {
                        trade.quantity = decision.volume;
                        portfolio_state.add_position(trade.quantity);

                        double to_pay = trade.quantity * row.price;
                        portfolio_state.remove_cash(to_pay);
                    } else if (
                        decision.signal == ticker::types::TradeSignal::Sell &&
                        decision.volume > 0
                    ) {
                        trade.quantity = decision.volume;
                        portfolio_state.remove_position(trade.quantity);

                        double to_receive = trade.quantity * row.price;
                        portfolio_state.add_cash(to_receive);

                    } else {
                        continue;
                    }

                    trade.cash_after = portfolio_state.get_cash();
                    trade.position_after = portfolio_state.get_position();

                    trade_log.push_back(trade);
                }

                report.ending_equity =
                    portfolio_state.get_cash() +
                    portfolio_state.get_position() * last_price;
                report.pnl = report.ending_equity - report.starting_cash;
                report.total_trades = trade_log.size();

                return report;
            }
    };
} // namespace ticker::backtest
