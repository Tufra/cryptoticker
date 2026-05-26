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


BYBIT_KLINE_URL = "https://api.bybit.com/v5/market/kline"
DATA_DIR = Path(__file__).resolve().parent.parent / "data"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Download Bybit kline data and save it as JSON under data/."
    )
    parser.add_argument("symbol", help="Trading pair, for example BTCUSDT or BTCUSD.")
    parser.add_argument(
        "interval",
        help="Bybit interval, for example 1, 3, 5, 60, 240, D, W, or M.",
    )
    parser.add_argument("limit", type=int, help="Number of klines to request.")
    return parser.parse_args()


def infer_category(symbol: str) -> str:
    normalized = symbol.upper()
    if normalized.endswith("USDT") or normalized.endswith("USDC"):
        return "linear"
    if normalized.endswith("USD"):
        return "inverse"
    return "spot"


def normalize_interval(interval: str) -> str:
    normalized = interval.strip()
    lower = normalized.lower()
    aliases = {
        "1m": "1",
        "3m": "3",
        "5m": "5",
        "15m": "15",
        "30m": "30",
        "1h": "60",
        "2h": "120",
        "4h": "240",
        "6h": "360",
        "12h": "720",
        "1d": "D",
        "1w": "W",
        "1mo": "M",
    }
    return aliases.get(lower, normalized)


def safe_filename_part(value: str) -> str:
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", value.strip())


def output_path(symbol: str, interval: str, limit: int) -> Path:
    filename = (
        f"bybit_{safe_filename_part(symbol.lower())}_"
        f"{safe_filename_part(interval)}_{limit}.json"
    )
    return DATA_DIR / filename


def fetch_klines(symbol: str, interval: str, limit: int, retries: int = 3) -> dict:
    params = {
        "category": infer_category(symbol),
        "symbol": symbol.upper(),
        "interval": normalize_interval(interval),
        "limit": str(limit),
    }
    url = f"{BYBIT_KLINE_URL}?{urlencode(params)}"
    request = Request(url, headers={"User-Agent": "cryptoticker/1.0"})

    for attempt in range(1, retries + 1):
        try:
            with urlopen(request, timeout=30) as response:
                payload = response.read().decode("utf-8")
            break
        except HTTPError as error:
            if error.code < 500 or attempt == retries:
                raise RuntimeError(
                    f"Bybit returned HTTP {error.code}: {error.reason}"
                ) from error
        except URLError as error:
            if attempt == retries:
                raise RuntimeError(
                    f"Failed to connect to Bybit: {error.reason}"
                ) from error
        except TimeoutError as error:
            if attempt == retries:
                raise RuntimeError(f"Timed out while reading from Bybit") from error

        time.sleep(attempt)

    data = json.loads(payload)
    if data.get("retCode") != 0:
        message = data.get("retMsg", "unknown error")
        raise RuntimeError(f"Bybit API error: {message}")

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
