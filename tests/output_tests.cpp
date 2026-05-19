import ticker.types;
import ticker.output;

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

    ticker::output::write_to_csv(output, data);

    std::string expected = "timestamp,price,sma,vol,signal\n"
                           "1,1.00000000,10.00000000,100.00000000,Buy\n"
                           "2,2.00000000,20.00000000,200.00000000,Hold\n"
                           "3,3.00000000,30.00000000,300.00000000,Sell\n"
                           "4,4.00000000,,,\n";

    REQUIRE(output.str() == expected);
}
