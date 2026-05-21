import ticker.types;
import ticker.output;
import ticker.backtest.types;

#include <catch2/catch_test_macros.hpp>
#include <format>
#include <sstream>

TEST_CASE("Write Price Ticks to CSV", "[output]") {
    auto output = std::ostringstream();

    auto data = std::vector<ticker::types::StatsRow>{
        {1, 1.0, 10, 100, ticker::types::TradeSignal::Buy},
        {2, 2.0, 20, 200, ticker::types::TradeSignal::Hold},
        {3, 3.0, 30, 300, ticker::types::TradeSignal::Sell},
        {4, 4.0, std::nullopt, std::nullopt, std::nullopt}
    };

    auto total = ticker::output::write_ticks_to_csv(output, data);

    std::string expected = "timestamp,price,sma,vol,signal\n"
                           "1,1.00000000,10.00000000,100.00000000,Buy\n"
                           "2,2.00000000,20.00000000,200.00000000,Hold\n"
                           "3,3.00000000,30.00000000,300.00000000,Sell\n"
                           "4,4.00000000,,,\n";

    REQUIRE(output.str() == expected);
    REQUIRE(total == 4);
}

TEST_CASE("Write Backtest Trades to CSV", "[output]") {
    auto output = std::ostringstream();

    auto data = std::vector<ticker::backtest::types::Trade>{
        {1, ticker::types::TradeSignal::Buy, 1.0, 10, 100, 100, 0},
        {2, ticker::types::TradeSignal::Hold, 2.0, 20, 200, 200, 0},
        {3, ticker::types::TradeSignal::Sell, 3.0, 30, 300, 300, 0}
    };

    auto total = ticker::output::write_backtest_trades_to_csv(output, data);

    std::string expected = "timestamp,signal,price,quantity,cash_after,"
                           "position_after,commission\n"
                           "1,Buy,1.00000000,10.00000000,100.00000000,"
                           "100.00000000,0.00000000\n"
                           "2,Hold,2.00000000,20.00000000,200.00000000,"
                           "200.00000000,0.00000000\n"
                           "3,Sell,3.00000000,30.00000000,300.00000000,"
                           "300.00000000,0.00000000\n";

    REQUIRE(output.str() == expected);
    REQUIRE(total == 3);
}
