#!/usr/bin/env python3
"""Minimal benchmark summary validator."""

from __future__ import annotations

import csv
import sys
from pathlib import Path


REQUIRED_COLUMNS = {
    "algorithm",
    "pattern",
    "split",
    "count",
    "element_size",
    "dataset_seed",
    "sampled_pair_count",
    "disorder_score",
    "equal_score",
    "equal_pair_count",
    "has_less",
    "has_greater",
    "direction_changes",
    "timing_repetition",
    "elapsed_ticks",
    "correctness",
}


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: validate_benchmark.py summary.csv", file=sys.stderr)
        return 2
    path = Path(argv[1])
    with path.open("r", newline="", encoding="utf-8-sig") as fh:
        rows = list(csv.DictReader(fh))
    if not rows:
        print("empty summary", file=sys.stderr)
        return 1
    missing = REQUIRED_COLUMNS.difference(rows[0].keys())
    if missing:
        print(f"missing columns: {sorted(missing)}", file=sys.stderr)
        return 1
    splits = set()
    seeds_by_split = {}
    for row in rows:
        split = row["split"].strip().lower()
        splits.add(split)
        seeds_by_split.setdefault(split, set()).add(row["dataset_seed"].strip())
        if row["correctness"].strip().lower() != "ok":
            print("found incorrect row", file=sys.stderr)
            return 1
    if not {"train", "validation", "test"}.issubset(splits):
        print(f"missing required split(s): expected train, validation, and test, saw {sorted(splits)}", file=sys.stderr)
        return 1
    split_names = ["train", "validation", "test"]
    for i, left in enumerate(split_names):
        for right in split_names[i + 1:]:
            if seeds_by_split.get(left, set()) & seeds_by_split.get(right, set()):
                print(f"overlapping seeds between {left} and {right}", file=sys.stderr)
                return 1
    print(f"validated_rows={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
