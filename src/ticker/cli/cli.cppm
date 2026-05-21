module;

#include <iostream>
#include <stdexcept>
#include <string>

export module ticker.cli;
import ticker.cli.types;
import ticker.types;

export namespace ticker::cli {
    const auto DEFAULT_INPUT_NAME = "input.csv";
    const auto DEFAULT_OUTPUT_NAME = "output.csv";
    const auto DEFAULT_WINDOW_SIZE = 5;
    const auto DEFAULT_SOURCE = ticker::types::DataSource::Bybit;
    const auto DEFAULT_MODE = ticker::types::Mode::Backtest;

    const std::string &
    require_value(const std::string &value, const std::string &name) {
        if (value.empty()) {
            throw std::runtime_error("Value for " + name + " is required");
        } else if (
            value.starts_with("--") || value == "-i" || value == "-o" ||
            value == "-w" || value == "-h" || value == "-s"
        ) {
            throw std::runtime_error("Value cannot start with -");
        }
        return value;
    }

    ticker::cli::types::ProgramParameters
    parse_args(const size_t argc, const char *argv[]) {
        ticker::cli::types::ProgramParameters config{
            .source = DEFAULT_SOURCE,
            .mode = DEFAULT_MODE,
            .input = DEFAULT_INPUT_NAME,
            .output = DEFAULT_OUTPUT_NAME,
            .window_size = DEFAULT_WINDOW_SIZE,
            .is_help = false
        };

        for (size_t i = 1; i < argc; i++) {
            auto arg = std::string(argv[i]);

            if (arg == "--input" || arg == "-i") {
                auto val = (i < argc - 1 ? argv[++i] : "");
                config.input = require_value(val, "input");
            } else if (arg == "--output" || arg == "-o") {
                auto val = (i < argc - 1 ? argv[++i] : "");
                config.output = require_value(val, "output");
            } else if (arg == "--window-size" || arg == "-w") {
                auto val = (i < argc - 1 ? argv[++i] : "");
                config.window_size = std::stoi(require_value(val, "window"));
                if (config.window_size <= 0) {
                    throw std::runtime_error(
                        "Window size must be a positive integer"
                    );
                }
            } else if (arg == "--help" || arg == "-h") {
                config.is_help = true;
            } else if (arg == "--source" || arg == "-s") {
                auto val = (i < argc - 1 ? argv[++i] : "");
                config.source = ticker::types::str_to_data_source(
                    require_value(val, "source")
                );
            } else if (arg == "--mode" || arg == "-m") {
                auto val = (i < argc - 1 ? argv[++i] : "");
                config.mode =
                    ticker::types::str_to_mode(require_value(val, "mode"));
            } else {
                throw std::runtime_error("Unknown argument: " + arg);
            }
        }

        return config;
    }

    void print_help() {
        std::cout << "Usage: cryptoticker --source [source] --input "
                     "[input_file] --output "
                     "[output_file] --window-size [window_size] --mode "
                     "[backtest|ticks]"
                  << '\n';
    }

} // namespace ticker::cli
