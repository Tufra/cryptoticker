import ticker.types;
import ticker.stats;
import ticker.binance.input;

#include <filesystem>
#include <iostream>
#include <ranges>

int main(int argc, char** argv) {
    std::filesystem::path filename = "data/btcusdt_1m.json";
    auto output = std::ofstream("data/output/output.csv");

    auto entries = ticker::binance::input::read_binance_price_data(filename);
    auto price_ticks =
        entries |
        std::views::transform(ticker::binance::input::data_entry_to_tick);

    auto window_stats = ticker::stats::MovingWindowStats(5);

    output << "timestamp,price,sma,vol\n";
    for (const auto& entry : price_ticks) {
        window_stats.push_tick(entry);
        auto sma = window_stats.get_sma();
        auto vol = window_stats.get_volatility();

        output << entry.timestamp << "," << entry.price << ","
               << (sma.has_value() ? std::to_string(sma.value()) : "") << ","
               << (vol.has_value() ? std::to_string(vol.value()) : "") << "\n";
    }

    output.close();

    return 0;
}
