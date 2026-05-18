import ticker.types;
import ticker.stats;
import ticker.strategy;
import ticker.output;
import ticker.cli;
import ticker.binance.input;
import ticker.bybit.input;

#include <iostream>
#include <ranges>
#include <vector>

int main(const int argc, const char *argv[]) {
    try {
        auto params = ticker::cli::parse_args(argc, argv);
        auto input_path = params.input;
        auto output_path = params.output;
        auto window_size = params.window_size;
        if (params.is_help) {
            ticker::cli::print_help();
            return 0;
        }

        auto entries =
            ticker::bybit::input::read_bybit_price_data(input_path);
        auto price_ticks =
            entries |
            std::views::transform(ticker::bybit::input::data_entry_to_tick);

        auto window_stats = ticker::stats::MovingWindowStats(window_size);
        auto result = std::vector<ticker::types::StatsRow>();

        for (const auto &entry : price_ticks) {
            window_stats.push_tick(entry);
            auto price = entry.price;
            auto sma = window_stats.get_sma();
            auto vol = window_stats.get_volatility();

            auto signal = sma.has_value() && vol.has_value()
                              ? std::optional{ticker::strategy::define_signal(
                                    price, sma.value(), vol.value()
                                )}
                              : std::nullopt;

            auto row = ticker::types::StatsRow{
                entry.timestamp, price, sma, vol, signal
            };
            result.push_back(row);
        }

        auto output = std::ofstream(output_path);
        if (!output.is_open()) {
            throw std::runtime_error("Failed to open file: " + output_path);
        }

        ticker::output::write_to_csv(output, result);
        output.close();

        std::cout << "Wrote " << result.size() << " entries to " << output_path
                  << " with window size " << window_size << '\n';

        return 0;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
