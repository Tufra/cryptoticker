import ticker.types;
import ticker.binance.input;

#include <filesystem>
#include <iostream>
#include <ranges>

int main(int argc, char** argv) {
    std::filesystem::path filename = "data/btcusdt_1m.json";

    auto entries = ticker::binance::input::read_binance_price_data(filename);
    auto price_ticks =
        entries |
        std::views::transform(ticker::binance::input::data_entry_to_tick);

    for (const auto& entry : price_ticks) {
        std::cout << entry.timestamp << " " << entry.price << std::endl;
    }

    return 0;
}
