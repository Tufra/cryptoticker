module;
#include <cmath>
#include <deque>
#include <numeric>
#include <optional>
#include <span>
#include <stdexcept>

export module ticker.stats;
import ticker.types;

export namespace ticker::stats {
    class MovingWindowStats {
        private:
            size_t window_size;
            std::deque<ticker::types::PriceTick> tick_queue;

        public:
            MovingWindowStats(size_t window_size) : window_size(window_size) {
                if (window_size == 0) {
                    throw std::invalid_argument(
                        "Window size must be greater than 0"
                    );
                }
            }
            MovingWindowStats(
                size_t window_size,
                std::span<const ticker::types::PriceTick> window
            )
                : window_size(window_size) {
                if (window_size == 0) {
                    throw std::invalid_argument(
                        "Window size must be greater than 0"
                    );
                }

                for (const auto &tick : window) {
                    push_tick(tick);
                }
            }

            const std::deque<ticker::types::PriceTick> &get_window() const {
                return tick_queue;
            }

            size_t get_window_size() const { return window_size; }

            void push_tick(ticker::types::PriceTick tick) {
                tick_queue.push_back(tick);
                if (tick_queue.size() > window_size) {
                    tick_queue.pop_front();
                }
            }

            std::optional<double> get_sma() const {
                if (tick_queue.empty() || tick_queue.size() < window_size) {
                    return std::nullopt;
                }
                auto sum = std::accumulate(
                    tick_queue.begin(), tick_queue.end(), 0.0,
                    [](double acc, const ticker::types::PriceTick &tick) {
                        return acc + tick.price;
                    }
                );
                return sum / tick_queue.size();
            }

            std::optional<double> get_volatility() const {
                std::optional<double> sma = get_sma();
                if (!sma) {
                    return std::nullopt;
                }
                double sma_value = sma.value();
                double diff = std::accumulate(
                    tick_queue.begin(), tick_queue.end(), 0.0,
                    [sma_value](
                        double acc, const ticker::types::PriceTick &tick
                    ) { return acc + std::pow(tick.price - sma_value, 2); }
                );

                return std::sqrt(diff / tick_queue.size());
            }
    };

    std::span<const ticker::types::StatsRow> get_history_window(
        std::span<const ticker::types::StatsRow> stats,
        std::size_t index,
        std::size_t length
    ) {
        const std::size_t end = index + 1;
        const std::size_t start = end > length ? end - length : 0;

        return stats.subspan(start, end - start);
    };
} // namespace ticker::stats
