import ticker.types;
import ticker.strategy;
import ticker.strategy.types;
import ticker.runner;
import ticker.runner.types;
import ticker.executor;
import ticker.risk_manager;
import ticker.risk_manager.types;

#include <catch2/catch_test_macros.hpp>

class MockStrategy : public ticker::strategy::AbstractStrategy {
    public:
        ticker::strategy::types::StrategyDecision decide(
            const ticker::strategy::types::StrategyContext &context
        ) const override {
            const auto row = context.history.back();
            const auto portfolio_state = context.portfolio_state;

            if (row.timestamp == 1) {
                return ticker::strategy::types::StrategyDecision{
                    .timestamp = row.timestamp,
                    .signal = ticker::types::TradeSignal::Buy,
                    .volume = portfolio_state.get_cash() / row.price,
                    .price = row.price
                };
            } else if (row.timestamp == 2) {
                return ticker::strategy::types::StrategyDecision{
                    .timestamp = row.timestamp,
                    .signal = ticker::types::TradeSignal::Sell,
                    .volume = portfolio_state.get_position(),
                    .price = row.price
                };
            }

            return ticker::strategy::types::StrategyDecision{
                .timestamp = row.timestamp,
                .signal = ticker::types::TradeSignal::Hold,
                .volume = 0,
                .price = row.price
            };
        }
};

TEST_CASE("Backtest on simple data", "[backtest]") {
    auto price_ticks = std::vector<ticker::types::StatsRow>{
        {.timestamp = 1, .price = 10}, {.timestamp = 2, .price = 20}
    };
    double starting_cash = 100;
    auto strategy = MockStrategy();
    auto portfolio_state = ticker::types::PortfolioState(starting_cash);
    auto executor = ticker::executor::SimulationExecutor(portfolio_state);
    auto risk_manager = ticker::risk_manager::SimpleRiskManager(1, 1, 1);

    auto backtest_engine = ticker::runner::Runner(
        strategy, risk_manager, executor, portfolio_state
    );

    auto backtest_report = backtest_engine.run(price_ticks);

    REQUIRE(backtest_report.starting_cash == starting_cash);
    REQUIRE(backtest_report.total_trades == 2);
    REQUIRE(backtest_report.ending_equity == 200);
    REQUIRE(backtest_report.pnl == 100);
}

TEST_CASE("Backtest with open position", "[backtest]") {
    auto price_ticks = std::vector<ticker::types::StatsRow>{
        {.timestamp = 1, .price = 10}, {.timestamp = 3, .price = 20}
    };
    double starting_cash = 100;
    auto strategy = MockStrategy();
    auto portfolio_state = ticker::types::PortfolioState(starting_cash);
    auto executor = ticker::executor::SimulationExecutor(portfolio_state);
    auto risk_manager = ticker::risk_manager::SimpleRiskManager(1, 1, 1);

    auto backtest_engine = ticker::runner::Runner(
        strategy, risk_manager, executor, portfolio_state
    );

    auto backtest_report = backtest_engine.run(price_ticks);

    REQUIRE(backtest_report.starting_cash == starting_cash);
    REQUIRE(backtest_report.total_trades == 1);
    REQUIRE(backtest_report.ending_equity == 200);
    REQUIRE(backtest_report.pnl == 100);
}
