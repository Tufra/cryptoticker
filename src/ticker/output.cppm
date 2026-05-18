module;

#include <iomanip>
#include <optional>
#include <ostream>
#include <span>

export module ticker.output;
import ticker.types;

export namespace ticker::output {

    void write_header(std::ostream &stream) {
        stream << "timestamp" << "," << "price" << "," << "sma" << "," << "vol"
               << "," << "signal" << '\n';
    }

    void
    write_optional_double(std::ostream &stream, std::optional<double> value) {
        if (value.has_value()) {
            stream << value.value();
        }
    }

    void write_to_csv(
        std::ostream &stream, std::span<const ticker::types::StatsRow> stats
    ) {
        stream << std::fixed << std::setprecision(8);
        write_header(stream);

        for (const auto &row : stats) {
            auto sma = row.sma;
            auto vol = row.volatility;
            auto signal = row.signal;

            stream << row.timestamp << "," << row.price << ",";
            write_optional_double(stream, sma);
            stream << ",";
            write_optional_double(stream, vol);
            stream << ","
                   << (signal.has_value()
                           ? ticker::types::signal_to_string(signal.value())
                           : "")
                   << "\n";
        }
    }

} // namespace ticker::output
