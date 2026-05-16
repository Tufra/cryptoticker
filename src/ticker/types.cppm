export module ticker.types;

export import ticker.binance.types;

export namespace ticker::types {

struct PriceTick {
    double price;
    long long timestamp;
};

}  // namespace ticker::types