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
            src/ticker/strategy.cppm
            src/ticker/log.cppm
            src/ticker/output.cppm

            src/ticker/binance/types.cppm
            src/ticker/binance/input.cppm
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
    tests/stats/stats_tests.cpp
    tests/strategy/strategy_tests.cpp
)

target_link_libraries(cryptoticker_tests
    PRIVATE
        Catch2::Catch2WithMain
        cryptoticker_lib
)

include(CTest)
include(Catch)
catch_discover_tests(cryptoticker_tests)

