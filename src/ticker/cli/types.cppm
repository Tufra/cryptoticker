module;

#include <stdexcept>
#include <string>

export module ticker.cli.types;
import ticker.types;

export namespace ticker::cli::types {

    struct ProgramParameters {
        public:
            ticker::types::DataSource source;
            std::string input;
            std::string output;
            std::size_t window_size;
            bool is_help;
    };

} // namespace ticker::cli::types
