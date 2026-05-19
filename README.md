# CryptoTicker

CryptoTicker is a command-line market data processor for local
cryptocurrency candle datasets. It ingests kline JSON, normalizes market
data into internal price ticks, computes rolling statistics, derives trading
signals, and writes deterministic CSV output suitable for downstream analysis.

## Features

- Kline JSON ingestion from local files.
- Conversion from exchange-specific candle records to internal price ticks.
- Rolling simple moving average (SMA).
- Rolling volatility.
- Signal generation: `Buy`, `Sell`, `Hold`.
- CSV output with fixed 8-decimal floating-point formatting.
- CLI configuration for input path, output path, and rolling window size.

## Requirements

Recommended environment:

- CMake 3.28+
- Clang/LLVM 22
- Boost.JSON

## Build

Configure:

```bash
CC=clang-22 CXX=clang++-22 cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER_CLANG_SCAN_DEPS=/usr/bin/clang-scan-deps-22
```

Build:

```bash
cmake --build build
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

## Usage

```bash
./build/cryptoticker \
  --source binance \
  --input data/btcusdt_1m.json \
  --output data/output/output.csv \
  --window-size 5
```

Short flags are supported:

```bash
./build/cryptoticker \
  -s binance \
  -i data/btcusdt_1m.json \
  -o data/output/output.csv \
  -w 5
```

Show help:

```bash
./build/cryptoticker --help
```

## CLI Options

| Option          | Short | Description                    | Default      |
| --------------- | ----- | ------------------------------ | ------------ |
| `--source`      | `-s`  | Kline data format              | `bybit`      |
| `--input`       | `-i`  | Input kline JSON file          | `input.csv`  |
| `--output`      | `-o`  | Output CSV file                | `output.csv` |
| `--window-size` | `-w`  | Rolling statistics window size | `5`          |
| `--help`        | `-h`  | Print usage information        |              |

Invalid arguments are reported with a non-zero exit code.

## Input Format

The input reader expects kline JSON file in the format returned by the
specified market data endpoint.

Supported sources:

- Binance
- Bybit

Example data acquisition (Binance):

```bash
curl "https://api.binance.com/api/v3/klines?symbol=BTCUSDT&interval=1m&limit=1000" \
  -o data/btcusdt_1m.json
```

## Output Format

CryptoTicker writes CSV with the following columns:

```csv
timestamp,price,sma,vol,signal
```

Example:

```csv
timestamp,price,sma,vol,signal
1778853539999,78830.31000000,,,
1778853599999,78758.59000000,,,
1778853659999,78719.42000000,,,
1778853719999,78845.78000000,,,
1778853779999,78949.35000000,78820.69000000,79.23063928,Buy
```

`sma`, `vol`, and `signal` are empty until the rolling window contains enough
ticks.

## Signal Model

For each ready rolling window:

- `Buy` when `price > sma + volatility`
- `Sell` when `price < sma - volatility`
- `Hold` otherwise

This strategy is intentionally deterministic and transparent. It is designed as
a baseline signal model that can be replaced or extended without changing the
input, statistics, or output layers.
