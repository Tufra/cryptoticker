export module ticker.strategy;

import ticker.types;

export namespace ticker::strategy {

    ticker::types::TradeSignal
    define_signal(double price, double sma, double volatility) {
        if (price > sma + volatility) {
            return ticker::types::TradeSignal::Buy;
        } else if (price < sma - volatility) {
            return ticker::types::TradeSignal::Sell;
        } else {
            return ticker::types::TradeSignal::Hold;
        }
    }
} // namespace ticker::strategy
