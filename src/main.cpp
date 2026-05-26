#include <iostream>
#include <vector>

import ticker.types;
import ticker.stats;
import ticker.strategy;
import ticker.strategy.types;
import ticker.risk_manager;
import ticker.output;
import ticker.cli;
import ticker.cli.types;
import ticker.input;
import ticker.runner;
import ticker.runner.types;
import ticker.executor;


int main(const int argc, const char *argv[]) {
    try {
        auto params = ticker::cli::parse_args(argc, argv);
        auto input_path = params.input;
        auto output_path = params.output;
        auto window_size = params.window_size;
        auto source = params.source;
        auto mode = params.mode;

        if (params.is_help) {
            ticker::cli::print_help();
            return 0;
        }

        auto price_ticks = ticker::input::read_market_data(source, input_path);

        auto window_stats = ticker::stats::MovingWindowStats(window_size);
        auto result = std::vector<ticker::types::StatsRow>();

        auto simple_strategy = ticker::strategy::SimpleStrategy();
        auto simple_risk_manager =
            ticker::risk_manager::SimpleRiskManager(0.1, 0.1, 0.3);
        auto portfolio_state = ticker::types::PortfolioState(10000);
        auto simulation_executor =
            ticker::executor::SimulationExecutor(portfolio_state);

        for (const auto &entry : price_ticks) {
            window_stats.push_tick(entry);
            auto price = entry.price;
            auto sma = window_stats.get_sma();
            auto vol = window_stats.get_volatility();

            auto row =
                ticker::types::StatsRow{entry.timestamp, price, sma, vol};

            result.push_back(row);
        }

        if (mode == ticker::types::Mode::Ticks) {
            for (std::size_t i = 0; i < result.size(); i++) {
                const auto history =
                    ticker::stats::get_history_window(result, i, window_size);

                auto context = ticker::strategy::types::StrategyContext{
                    .history = history, .portfolio_state = portfolio_state
                };

                auto decision = simple_strategy.decide(context);

                result[i].signal = decision.signal;
            }

            auto output = std::ofstream(output_path);
            if (!output.is_open()) {
                throw std::runtime_error("Failed to open file: " + output_path);
            }

            auto total_entries =
                ticker::output::write_ticks_to_csv(output, result);
            output.close();

            std::cout << "Wrote " << total_entries << " entries to "
                      << output_path << " with window size " << window_size
                      << '\n';
        } else if (mode == ticker::types::Mode::Backtest) {
            auto backtest_engine = ticker::runner::Runner(
                simple_strategy, simple_risk_manager, simulation_executor,
                portfolio_state, window_size
            );

            auto backtest_report = backtest_engine.run(result);
            std::cout << backtest_report.to_string() << std::endl;

            auto trade_log = backtest_engine.get_trade_log();

            auto output = std::ofstream(output_path);
            if (!output.is_open()) {
                throw std::runtime_error("Failed to open file: " + output_path);
            }

            auto total_entries =
                ticker::output::write_trades_to_csv(output, trade_log);
            output.close();

            std::cout << "Wrote " << total_entries << " entries to "
                      << output_path << " with window size " << window_size
                      << '\n';
        } else {
            throw std::runtime_error("Unknown mode");
        }

        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
