import ticker.input;
import ticker.types;

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

const auto BINANCE_INPUT_PATH = "../tests/data/binance_btcusdt_1m.json";
const auto BYBIT_INPUT_PATH = "../tests/data/bybit_btcusd_1m.json";

TEST_CASE("Universal Read Binance Price Data from JSON file", "[input]") {
    auto source = ticker::types::DataSource::Binance;

    auto result = ticker::input::read_market_data(source, BINANCE_INPUT_PATH);

    REQUIRE(result.size() == 1000);
    REQUIRE(result[0].timestamp == 1778853539999);
    REQUIRE(result[0].price == 78830.31);
}

TEST_CASE("Universal Read Bybit Price Data from JSON file", "[input]") {
    auto source = ticker::types::DataSource::Bybit;

    auto result = ticker::input::read_market_data(source, BYBIT_INPUT_PATH);

    REQUIRE(result.size() == 500);
    REQUIRE(result[0].timestamp == 1779093720000);
    REQUIRE(result[0].price == 76882.6);
}

TEST_CASE("Fail to read with wrong source", "[input]") {
    auto binance_source = ticker::types::DataSource::Binance;

    REQUIRE_THROWS(ticker::input::read_market_data(binance_source, BYBIT_INPUT_PATH));

    auto bybit_source = ticker::types::DataSource::Bybit;

    REQUIRE_THROWS(ticker::input::read_market_data(bybit_source, BINANCE_INPUT_PATH));
}
