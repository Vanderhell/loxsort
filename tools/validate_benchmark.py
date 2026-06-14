#!/usr/bin/env python3
"""Minimal benchmark summary validator."""

from __future__ import annotations

import csv
import sys
from pathlib import Path


REQUIRED_COLUMNS = {
    "algorithm",
    "pattern",
    "count",
    "element_size",
    "dataset_seed",
    "timing_repetition",
    "elapsed_ticks",
    "correctness",
}


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: validate_benchmark.py summary.csv", file=sys.stderr)
        return 2
    path = Path(argv[1])
    with path.open("r", newline="", encoding="utf-8") as fh:
        rows = list(csv.DictReader(fh))
    if not rows:
        print("empty summary", file=sys.stderr)
        return 1
    missing = REQUIRED_COLUMNS.difference(rows[0].keys())
    if missing:
        print(f"missing columns: {sorted(missing)}", file=sys.stderr)
        return 1
    for row in rows:
        if row["correctness"].strip().lower() != "ok":
            print("found incorrect row", file=sys.stderr)
            return 1
    print(f"validated_rows={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))

