export module ticker.types;

export import ticker.binance.types;

export namespace ticker::types {

struct PriceTick {
    double price;
    long long timestamp;

    bool operator==(const PriceTick& other) const {
        return price == other.price && timestamp == other.timestamp;
    }
};

}  // namespace ticker::types
