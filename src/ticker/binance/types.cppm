export module ticker.binance.types;

export namespace ticker::binance::types {

struct BinanceKlineDataEntry {
    long long open_time;
    double open_price;
    double high_price;
    double low_price;
    double close_price;
    double volume;
    long long close_time;
    double quote_asset_volume;
    long long number_of_trades;
    double taker_buy_base_asset_volume;
    double taker_buy_quote_asset_volume;
};

}  // namespace ticker::binance::types
