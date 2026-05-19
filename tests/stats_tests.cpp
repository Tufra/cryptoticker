import ticker.types;
import ticker.stats;

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cmath>

auto now = std::chrono::system_clock::now();
auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch())
                  .count();

std::vector values{ticker::types::PriceTick{100, now_ms},
                   ticker::types::PriceTick{110, now_ms + 1000},
                   ticker::types::PriceTick{120, now_ms + 2000},
                   ticker::types::PriceTick{130, now_ms + 3000},
                   ticker::types::PriceTick{140, now_ms + 4000}};

TEST_CASE("Create Window", "[stats]") {
    auto window = ticker::stats::MovingWindowStats(5);
    REQUIRE(window.get_window_size() == 5);
    REQUIRE(window.get_window().empty());

    auto filled_window = ticker::stats::MovingWindowStats(5, values);
    auto created_window = filled_window.get_window();

    REQUIRE(filled_window.get_window_size() == 5);
    REQUIRE(created_window[0] == values[0]);
    REQUIRE(created_window.end()[-1] == values[4]);
}

TEST_CASE("SMA", "[stats]") {
    auto filled_window = ticker::stats::MovingWindowStats(4);
    filled_window.push_tick(values[0]);
    filled_window.push_tick(values[1]);
    filled_window.push_tick(values[2]);
    filled_window.push_tick(values[3]);

    auto sma = filled_window.get_sma();
    REQUIRE(sma.value() == (100.0 + 110.0 + 120.0 + 130.0) / 4.0);

    filled_window.push_tick(values[4]);
    sma = filled_window.get_sma();
    REQUIRE(sma.value() == (110.0 + 120.0 + 130.0 + 140.0) / 4.0);
}

TEST_CASE("Volatility", "[stats]") {
    auto filled_window = ticker::stats::MovingWindowStats(4);
    filled_window.push_tick(values[0]);
    filled_window.push_tick(values[1]);
    filled_window.push_tick(values[2]);
    filled_window.push_tick(values[3]);

    auto volatility = filled_window.get_volatility();
    auto avg = (100.0 + 110.0 + 120.0 + 130.0) / 4.0;
    auto test_vol =
        std::sqrt((std::pow(100 - avg, 2) + std::pow(110 - avg, 2) +
                   std::pow(120 - avg, 2) + std::pow(130 - avg, 2)) /
                  4.0);

    REQUIRE(volatility.value() == test_vol);
}
