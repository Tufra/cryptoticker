export module ticker.bybit.types;

export namespace ticker::bybit::types {

    struct BybitKlineDataEntry {
        public:
            long long open_time;
            double open_price;
            double high_price;
            double low_price;
            double close_price;
            double volume;
            double turnover;
    };

} // namespace ticker::bybit::types
