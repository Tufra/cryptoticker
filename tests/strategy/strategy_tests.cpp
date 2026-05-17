import ticker.types;
import ticker.stats;
import ticker.strategy;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Define Signal", "[strategy]") {
    auto price = 100.0;
    auto sma = 110.0;
    auto volatility = 10.0;

    auto signal = ticker::strategy::define_signal(price, sma, volatility);
    REQUIRE(signal == ticker::types::TradeSignal::Hold);

    price = 120.0;
    sma = 110.0;
    volatility = 5.0;

    signal = ticker::strategy::define_signal(price, sma, volatility);
    REQUIRE(signal == ticker::types::TradeSignal::Buy);

    price = 80.0;
    sma = 110.0;
    volatility = 20.0;

    signal = ticker::strategy::define_signal(price, sma, volatility);
    REQUIRE(signal == ticker::types::TradeSignal::Sell);
}
