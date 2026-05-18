import ticker.bybit.input;
import ticker.bybit.types;
import ticker.types;

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Read Bybit Price Data from JSON file", "[input]") {
    auto input = ticker::bybit::input::read_bybit_price_data(
        "../data/bybit_btcusd_1m.json");

    REQUIRE(input.size() == 500);
    REQUIRE(input[0].open_time == 1779093720000);
    REQUIRE(input[0].open_price == 76836.2);
}

TEST_CASE("Convert Bybit Price Data Entry to Price Tick", "[input]") {
    auto kline_entry = ticker::bybit::types::BybitKlineDataEntry{
        1778853480000,
        std::stod("78769.77000000"),
        std::stod("78841.98000000"),
        std::stod("78698.69000000"),
        std::stod("78830.31000000"),
        std::stod("65.90742000"),
        std::stod("36.85455000")
    };

    auto tick = ticker::bybit::input::data_entry_to_tick(kline_entry);

    REQUIRE(tick.timestamp == 1778853480000);
    REQUIRE(tick.price == 78830.31);
}
