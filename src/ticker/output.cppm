module;

#include <fstream>
#include <span>

export module ticker.output;
import ticker.types;

export namespace ticker::output {

    void write_header(std::ofstream &stream) {
        stream << "timestamp" << "," << "price" << "," << "sma" << "," << "vol"
               << "," << "signal" << '\n';
    }

    void write_to_csv(
        std::ofstream &stream, std::span<const ticker::types::StatsRow> stats
    ) {
        write_header(stream);

        for (const auto &row : stats) {
            auto sma = row.sma;
            auto vol = row.volatility;
            auto signal = row.signal;

            stream << row.timestamp << "," << row.price << ","
                   << (sma.has_value() ? std::to_string(sma.value()) : "")
                   << ","
                   << (vol.has_value() ? std::to_string(vol.value()) : "")
                   << ","
                   << (signal.has_value()
                           ? ticker::types::signal_to_string(signal.value())
                           : "")
                   << "\n";
        }
    }

} // namespace ticker::output
