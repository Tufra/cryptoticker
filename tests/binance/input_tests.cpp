import ticker.binance.input;
import ticker.binance.types;
import ticker.types;

#include <catch2/catch_test_macros.hpp>

const auto INPUT_PATH = "../tests/data/binance_btcusdt_1m.json";

TEST_CASE("Read Binance Price Data from JSON file", "[binance]") {
    auto input = ticker::binance::input::read_price_data(INPUT_PATH);

    REQUIRE(input.size() == 1000);
    REQUIRE(input[0].open_time == 1778853480000);
    REQUIRE(input[0].open_price == 78769.77);
}

TEST_CASE("Convert Binance Price Data Entry to Price Tick", "[binance]") {
    auto kline_entry = ticker::binance::types::BinanceKlineDataEntry{
        1778853480000,
        std::stod("78769.77000000"),
        std::stod("78841.98000000"),
        std::stod("78698.69000000"),
        std::stod("78830.31000000"),
        std::stod("65.90742000"),
        1778853539999,
        std::stod("5190229.00970050"),
        9963,
        std::stod("36.85455000"),
        std::stod("2902862.09510340"),
    };

    auto tick = ticker::binance::input::data_entry_to_tick(kline_entry);

    REQUIRE(tick.timestamp == 1778853539999);
    REQUIRE(tick.price == 78830.31);
}
