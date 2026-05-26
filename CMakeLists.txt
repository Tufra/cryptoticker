cmake_minimum_required(VERSION 3.28)
include(FetchContent)

# CONFIG

project(cryptoticker
    VERSION 0.1
    DESCRIPTION "A cryptocurrency price ticker application"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# DEPENDENCIES

find_package(Boost REQUIRED COMPONENTS json)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.0
)
FetchContent_MakeAvailable(Catch2)

# LIB

add_library(cryptoticker_lib)

target_link_libraries(cryptoticker_lib
    PRIVATE
        Boost::json
)

target_sources(cryptoticker_lib
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
            src/ticker/input.cppm
            src/ticker/stats.cppm
            src/ticker/types.cppm
            src/ticker/log.cppm
            src/ticker/output.cppm

            src/ticker/cli/cli.cppm
            src/ticker/cli/types.cppm

            src/ticker/binance/types.cppm
            src/ticker/binance/input.cppm

            src/ticker/bybit/types.cppm
            src/ticker/bybit/input.cppm

            src/ticker/runner/runner.cppm
            src/ticker/runner/types.cppm

            src/ticker/strategy/strategy.cppm
            src/ticker/strategy/types.cppm

            src/ticker/risk_manager/risk_manager.cppm
            src/ticker/risk_manager/types.cppm

            src/ticker/executor/executor.cppm
)

target_compile_options(cryptoticker_lib
    PRIVATE
        -Wall
        -Wextra
        -Wswitch-enum
)

# SOURCE

add_executable(cryptoticker)

target_link_libraries(cryptoticker
    PRIVATE
        cryptoticker_lib
)

target_sources(cryptoticker
    PRIVATE
        src/main.cpp
)

# TESTS

enable_testing()

add_executable(cryptoticker_tests
    tests/binance/input_tests.cpp
    tests/bybit/input_tests.cpp
    tests/cli/cli_tests.cpp
    tests/backtest/backtest_tests.cpp
    tests/risk_manager/simple_risk_manager_tests.cpp
    
    tests/stats_tests.cpp
    tests/output_tests.cpp
    tests/input_tests.cpp
)

target_link_libraries(cryptoticker_tests
    PRIVATE
        Catch2::Catch2WithMain
        cryptoticker_lib
)

include(CTest)
include(Catch)
catch_discover_tests(cryptoticker_tests)

