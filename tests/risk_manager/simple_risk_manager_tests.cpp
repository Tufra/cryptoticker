import ticker.types;
import ticker.strategy;
import ticker.strategy.types;
import ticker.runner;
import ticker.runner.types;
import ticker.executor;
import ticker.risk_manager;
import ticker.risk_manager.types;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Allows trades < max share", "[simple_risk_manager]") {
    const auto decision = ticker::strategy::types::StrategyDecision{
        .timestamp = 1,
        .signal = ticker::types::TradeSignal::Buy,
        .volume = 10,
        .price = 1
    };

    const auto portfolio_state = ticker::types::PortfolioState(1000);
    const auto context = ticker::strategy::types::StrategyContext{
        .history = std::vector<ticker::types::StatsRow>(),
        .portfolio_state = portfolio_state
    };


    auto simple_risk_manager =
        ticker::risk_manager::SimpleRiskManager(0.1, 0.1, 0.3);
    
    const auto result = simple_risk_manager.decide(context, decision);

    REQUIRE(result.type == ticker::risk_manager::types::RiskManagerDecisionType::Allow);
    REQUIRE(result.volume == 10);
    REQUIRE(result.signal == ticker::types::TradeSignal::Buy);
    REQUIRE(result.price == 1);
    REQUIRE(result.stop_loss.has_value());
    REQUIRE(result.take_profit.has_value());

}

TEST_CASE("Adjusts volume to max share", "[simple_risk_manager]") {
    const auto decision = ticker::strategy::types::StrategyDecision{
        .timestamp = 1,
        .signal = ticker::types::TradeSignal::Buy,
        .volume = 10,
        .price = 100
    };

    const auto portfolio_state = ticker::types::PortfolioState(1000);
    const auto context = ticker::strategy::types::StrategyContext{
        .history = std::vector<ticker::types::StatsRow>(),
        .portfolio_state = portfolio_state
    };


    auto simple_risk_manager =
        ticker::risk_manager::SimpleRiskManager(0.1, 0.1, 0.3);
    
    const auto result = simple_risk_manager.decide(context, decision);

    REQUIRE(result.type == ticker::risk_manager::types::RiskManagerDecisionType::Override);
    REQUIRE(result.volume == 1);
    REQUIRE(result.signal == ticker::types::TradeSignal::Buy);
    REQUIRE(result.price == 100);
    REQUIRE(result.stop_loss.has_value());
    REQUIRE(result.take_profit.has_value());
}
