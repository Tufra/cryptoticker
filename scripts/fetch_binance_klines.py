#!/usr/bin/env python3
import argparse
import json
import re
import sys
import time
from pathlib import Path
from urllib.error import HTTPError, URLError
from urllib.parse import urlencode
from urllib.request import Request, urlopen


BINANCE_KLINE_URLS = (
    "https://data-api.binance.vision/api/v3/klines",
    "https://api.binance.com/api/v3/klines",
)
DATA_DIR = Path(__file__).resolve().parent.parent / "data"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Download Binance kline data and save it as JSON under data/."
    )
    parser.add_argument("symbol", help="Trading pair, for example BTCUSDT.")
    parser.add_argument(
        "interval",
        help="Binance interval, for example 1m, 5m, 1h, 1d, 1w, or 1M.",
    )
    parser.add_argument("limit", type=int, help="Number of klines to request.")
    return parser.parse_args()


def safe_filename_part(value: str) -> str:
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", value.strip())


def output_path(symbol: str, interval: str, limit: int) -> Path:
    filename = (
        f"binance_{safe_filename_part(symbol.lower())}_"
        f"{safe_filename_part(interval)}_{limit}.json"
    )
    return DATA_DIR / filename


def fetch_klines(symbol: str, interval: str, limit: int, retries: int = 3) -> list:
    params = {
        "symbol": symbol.upper(),
        "interval": interval,
        "limit": str(limit),
    }
    last_error = None

    for base_url in BINANCE_KLINE_URLS:
        url = f"{base_url}?{urlencode(params)}"
        request = Request(url, headers={"User-Agent": "cryptoticker/1.0"})

        for attempt in range(1, retries + 1):
            try:
                with urlopen(request, timeout=30) as response:
                    payload = response.read().decode("utf-8")
                last_error = None
                break
            except HTTPError as error:
                details = error.read().decode("utf-8", errors="replace")
                last_error = RuntimeError(
                    f"Binance returned HTTP {error.code}: {details or error.reason}"
                )
                if error.code < 500 or attempt == retries:
                    raise last_error from error
            except URLError as error:
                last_error = RuntimeError(f"Failed to connect to Binance: {error.reason}")
                if attempt < retries:
                    time.sleep(attempt)

        if last_error is None:
            break

    if last_error is not None:
        raise last_error

    data = json.loads(payload)
    if isinstance(data, dict) and data.get("code"):
        raise RuntimeError(f"Binance API error: {data.get('msg', 'unknown error')}")

    if not isinstance(data, list):
        raise RuntimeError("Unexpected Binance response: expected a JSON array")

    return data


def main() -> int:
    args = parse_args()
    if args.limit <= 0:
        print("limit must be greater than zero", file=sys.stderr)
        return 2

    data = fetch_klines(args.symbol, args.interval, args.limit)

    DATA_DIR.mkdir(parents=True, exist_ok=True)
    path = output_path(args.symbol, args.interval, args.limit)
    path.write_text(json.dumps(data, indent=4), encoding="utf-8")

    print(path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
