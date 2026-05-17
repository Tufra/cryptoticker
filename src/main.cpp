import ticker.types;
import ticker.stats;
import ticker.strategy;
import ticker.output;
import ticker.binance.input;

#include <filesystem>
#include <iostream>
#include <ranges>
#include <vector>

int main(int argc, char **argv) {
    std::filesystem::path filename = "data/btcusdt_1m.json";

    auto entries = ticker::binance::input::read_binance_price_data(filename);
    auto price_ticks =
        entries |
        std::views::transform(ticker::binance::input::data_entry_to_tick);

    auto window_stats = ticker::stats::MovingWindowStats(5);

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

        auto row =
            ticker::types::StatsRow{entry.timestamp, price, sma, vol, signal};
        result.push_back(row);
    }

    std::string output_path = "data/output/output2.csv";

    auto output = std::ofstream(output_path);
    ticker::output::write_to_csv(output, result);

    output.close();

    std::cout << "Wrote " << result.size() << " entries to " << output_path
              << '\n';

    return 0;
}
