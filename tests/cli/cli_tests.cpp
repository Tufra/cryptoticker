import ticker.types;
import ticker.cli;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parse args", "[cli]") {
    const int argc = 8;
    const char *argv[argc] = {"./tests",     "-i", "input2.csv", "-o",
                              "output2.csv", "-w", "20",         "-h"};

    auto args = ticker::cli::parse_args(argc, argv);

    REQUIRE(args.input == "input2.csv");
    REQUIRE(args.output == "output2.csv");
    REQUIRE(args.window_size == 20);
    REQUIRE(args.is_help);
}

TEST_CASE("Use default args", "[cli]") {
    const int argc = 1;
    const char *argv[argc] = {"./tests"};

    auto args = ticker::cli::parse_args(argc, argv);

    REQUIRE(args.input == ticker::cli::DEFAULT_INPUT_NAME);
    REQUIRE(args.output == ticker::cli::DEFAULT_OUTPUT_NAME);
    REQUIRE(args.window_size == ticker::cli::DEFAULT_WINDOW_SIZE);
    REQUIRE(args.is_help == false);
}

TEST_CASE("Invalid args", "[cli]") {
    const int missing_arg_argc = 5;
    const char *missing_arg_argv[missing_arg_argc] = {
        "./tests", "--input", "--output", "-w", "20"
    };

    REQUIRE_THROWS(ticker::cli::parse_args(missing_arg_argc, missing_arg_argv));

    const int unknown_arg_argc = 5;
    const char *unknown_arg_argv[unknown_arg_argc] = {
        "./tests", "--sus", "amogus"
    };

    REQUIRE_THROWS(ticker::cli::parse_args(unknown_arg_argc, unknown_arg_argv));

    const int empty_window_argc = 5;
    const char *empty_window_argv[unknown_arg_argc] = {"./tests", "-w", "0"};

    REQUIRE_THROWS(ticker::cli::parse_args(unknown_arg_argc, unknown_arg_argv));
}
