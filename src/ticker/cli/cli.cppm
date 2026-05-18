module;

#include <iostream>
#include <stdexcept>
#include <string>

export module ticker.cli;
import ticker.cli.types;

export namespace ticker::cli {
    const auto DEFAULT_INPUT_NAME = "input.csv";
    const auto DEFAULT_OUTPUT_NAME = "output.csv";
    const auto DEFAULT_WINDOW_SIZE = 5;

    const std::string &
    require_value(const std::string &value, const std::string &name) {
        if (value.empty()) {
            throw std::runtime_error("Value for " + name + " is required");
        } else if (
            value.starts_with("--") || value == "-i" || value == "-o" ||
            value == "-w" || value == "-h"
        ) {
            throw std::runtime_error("Value cannot start with -");
        }
        return value;
    }

    ticker::cli::types::ProgramParameters
    parse_args(const size_t argc, const char *argv[]) {
        ticker::cli::types::ProgramParameters config{
            .input = DEFAULT_INPUT_NAME,
            .output = DEFAULT_OUTPUT_NAME,
            .window_size = DEFAULT_WINDOW_SIZE,
            .is_help = false
        };

        for (size_t i = 1; i < argc; i++) {
            auto arg = std::string(argv[i]);
            if (arg == "--input" || arg == "-i") {
                config.input =
                    require_value(i < argc - 1 ? argv[++i] : "", "input");
            } else if (arg == "--output" || arg == "-o") {
                config.output =
                    require_value(i < argc - 1 ? argv[++i] : "", "output");
            } else if (arg == "--window-size" || arg == "-w") {
                config.window_size = std::stoi(
                    require_value(i < argc - 1 ? argv[++i] : "", "window")
                );
            } else if (arg == "--help" || arg == "-h") {
                config.is_help = true;
            } else {
                throw std::runtime_error("Unknown argument: " + arg);
            }
        }

        return config;
    }

    void print_help() {
        std::cout << "Usage: cryptoticker --input file.json --output "
                     "file.csv --window-size 10"
                  << '\n';
    }

} // namespace ticker::cli
