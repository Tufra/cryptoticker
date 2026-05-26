module;

#include <span>
#include <vector>

export module ticker.runner;
import ticker.types;
import ticker.stats;
import ticker.strategy;
import ticker.strategy.types;
import ticker.risk_manager;
import ticker.risk_manager.types;
import ticker.executor;
import ticker.runner.types;

export namespace ticker::runner {
    class Runner {
        private:
            const ticker::strategy::AbstractStrategy &strategy;
            const ticker::risk_manager::AbstractRiskManager &risk_manager;
            ticker::executor::AbstractExecutor &executor;

            std::vector<ticker::types::Trade> trade_log;
            const ticker::types::PortfolioState &portfolio_state;

            std::size_t history_length;

        public:
            Runner(
                ticker::strategy::AbstractStrategy &strategy,
                ticker::risk_manager::AbstractRiskManager &risk_manager,
                ticker::executor::AbstractExecutor &executor,
                ticker::types::PortfolioState &portfolio_state,
                std::size_t history_length = 10
            )
                : strategy(strategy), risk_manager(risk_manager),
                  executor(executor), portfolio_state(portfolio_state),
                  history_length(history_length) {}

            const std::vector<ticker::types::Trade> &get_trade_log() const {
                return trade_log;
            }

            ticker::runner::types::RunnerReport
            run(std::span<const ticker::types::StatsRow> stats) {
                auto report = ticker::runner::types::RunnerReport{
                    .starting_cash = portfolio_state.get_cash(),
                    .ending_equity = portfolio_state.get_cash(),
                    .pnl = 0,
                    .total_trades = trade_log.size(),
                    .max_drawdown = 0
                };
                double last_price = 0;
                double peak_equity = portfolio_state.get_cash();
                double max_drawdown = 0;

                for (std::size_t i = 0; i < stats.size(); i++) {
                    const auto history = ticker::stats::get_history_window(
                        stats, i, history_length
                    );

                    const auto context =
                        ticker::strategy::types::StrategyContext{
                            .history = history,
                            .portfolio_state = portfolio_state
                        };

                    const auto strategy_decision = strategy.decide(context);

                    const auto risk_manager_decision =
                        risk_manager.decide(context, strategy_decision);
                    const auto trade_decision =
                        risk_manager_decision.to_trade_decision();

                    auto trade = executor.execute(trade_decision);
                    last_price = trade.price;

                    if (!trade.success) {
                        continue;
                    }

                    trade.cash_after = portfolio_state.get_cash();
                    trade.position_after = portfolio_state.get_position();

                    const auto current_equity =
                        portfolio_state.get_equity(last_price);

                    if (current_equity > peak_equity) {
                        peak_equity = current_equity;
                    }

                    double drawdown =
                        (peak_equity - current_equity) / peak_equity;
                    if (drawdown > max_drawdown) {
                        max_drawdown = drawdown;
                    }

                    trade_log.push_back(trade);
                }

                report.ending_equity = portfolio_state.get_equity(last_price);
                report.pnl = report.ending_equity - report.starting_cash;
                report.total_trades = trade_log.size();
                report.max_drawdown = max_drawdown;

                return report;
            }
    };
} // namespace ticker::runner
