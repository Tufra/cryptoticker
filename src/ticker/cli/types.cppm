module;

#include <string>

export module ticker.cli.types;

export namespace ticker::cli::types {

    struct ProgramParameters {
        public:
            std::string input;
            std::string output;
            std::size_t window_size;
            bool is_help;
    };

} // namespace ticker::cli::types
