#!/usr/bin/env python3
"""Run and verify a deterministic benchmark resume flow."""

from __future__ import annotations

import csv
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BENCH_EXE = ROOT / "build_release" / "Release" / "loxsort_bench.exe"
VALIDATOR = ROOT / "tools" / "validate_benchmark.py"
REPORT_PATH = ROOT / "generated" / "resume_verification_report.md"
PREFIX_CSV = ROOT / "generated" / "resume_prefix.csv"
FULL_CSV = ROOT / "generated" / "resume_full.csv"


def _run(args: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(args, cwd=ROOT, text=True, capture_output=True)


def _validate(path: Path) -> tuple[int, str, str]:
    proc = _run([sys.executable, str(VALIDATOR), str(path)])
    return proc.returncode, proc.stdout, proc.stderr


def _read_rows(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as fh:
        return list(csv.DictReader(fh))


def _dataset_ids(rows: list[dict[str, str]]) -> list[str]:
    ids = sorted({row["dataset_id"].strip() for row in rows}, key=lambda value: int(value))
    return ids


def _strategy_rows_by_dataset(rows: list[dict[str, str]]) -> dict[str, Counter[str]]:
    grouped: dict[str, Counter[str]] = defaultdict(Counter)
    for row in rows:
        grouped[row["dataset_id"].strip()][row["algorithm"].strip()] += 1
    return grouped


def main() -> int:
    if not BENCH_EXE.exists():
        print(f"missing benchmark executable: {BENCH_EXE}", file=sys.stderr)
        return 1

    prefix_args = [
        str(BENCH_EXE),
        "--campaign",
        "smoke",
        "--seed-base",
        "1",
        "--output",
        str(PREFIX_CSV),
        "--checkpoint-interval",
        "11",
        "--stop-after-datasets",
        "37",
        "--timing-target-ms",
        "2",
        "--timing-samples",
        "3",
    ]
    full_args = [
        str(BENCH_EXE),
        "--campaign",
        "smoke",
        "--seed-base",
        "1",
        "--output",
        str(FULL_CSV),
        "--checkpoint-interval",
        "11",
        "--timing-target-ms",
        "2",
        "--timing-samples",
        "3",
    ]

    prefix = _run(prefix_args)
    if prefix.returncode != 0:
        print(prefix.stdout)
        print(prefix.stderr, file=sys.stderr)
        return prefix.returncode

    resumed_args = prefix_args[:-6] + ["--resume", "--timing-target-ms", "2", "--timing-samples", "3"]
    resumed = _run(resumed_args)
    if resumed.returncode != 0:
        print(resumed.stdout)
        print(resumed.stderr, file=sys.stderr)
        return resumed.returncode

    full = _run(full_args)
    if full.returncode != 0:
        print(full.stdout)
        print(full.stderr, file=sys.stderr)
        return full.returncode

    resumed_rows = _read_rows(PREFIX_CSV)
    full_rows = _read_rows(FULL_CSV)

    resumed_validate_rc, resumed_validate_out, resumed_validate_err = _validate(PREFIX_CSV)
    full_validate_rc, full_validate_out, full_validate_err = _validate(FULL_CSV)
    if resumed_validate_rc != 0:
        print(resumed_validate_out)
        print(resumed_validate_err, file=sys.stderr)
        return resumed_validate_rc
    if full_validate_rc != 0:
        print(full_validate_out)
        print(full_validate_err, file=sys.stderr)
        return full_validate_rc

    resumed_ids = _dataset_ids(resumed_rows)
    full_ids = _dataset_ids(full_rows)
    resumed_counts = _strategy_rows_by_dataset(resumed_rows)
    full_counts = _strategy_rows_by_dataset(full_rows)
    expected_strategies = {
        "always_insertion",
        "always_shell",
        "always_intro",
        "always_merge_if_available",
        "always_cycle_if_available",
        "loxsort_dispatcher",
        "oracle_best",
    }

    duplicate_dataset_ids = [dataset_id for dataset_id, count in Counter(row["dataset_id"].strip() for row in resumed_rows).items() if count != 7]
    missing_dataset_ids = sorted(set(full_ids).difference(resumed_ids), key=lambda value: int(value))
    extra_dataset_ids = sorted(set(resumed_ids).difference(full_ids), key=lambda value: int(value))
    strategy_mismatch = [
        dataset_id
        for dataset_id in resumed_ids
        if set(resumed_counts[dataset_id].keys()) != expected_strategies or set(full_counts.get(dataset_id, Counter()).keys()) != expected_strategies
    ]

    digest_line = next((line for line in resumed_validate_out.splitlines() if line.startswith("campaign_ids=")), "campaign_ids=NOT AVAILABLE")
    report_lines = [
        "# Resume Verification Report",
        "",
        "## Commands",
        f"- prefix: {' '.join(prefix_args)}",
        f"- resume: {' '.join(resumed_args)}",
        f"- uninterrupted: {' '.join(full_args)}",
        "",
        "## Validation",
        f"- prefix return code: {prefix.returncode}",
        f"- resumed return code: {resumed.returncode}",
        f"- uninterrupted return code: {full.returncode}",
        f"- resumed validator: {resumed_validate_rc}",
        f"- uninterrupted validator: {full_validate_rc}",
        "",
        "## Comparison",
        f"- resumed row count: {len(resumed_rows)}",
        f"- uninterrupted row count: {len(full_rows)}",
        f"- resumed dataset count: {len(resumed_ids)}",
        f"- uninterrupted dataset count: {len(full_ids)}",
        f"- duplicate dataset ids in resumed output: {duplicate_dataset_ids if duplicate_dataset_ids else 'none'}",
        f"- missing dataset ids in resumed output: {missing_dataset_ids if missing_dataset_ids else 'none'}",
        f"- extra dataset ids in resumed output: {extra_dataset_ids if extra_dataset_ids else 'none'}",
        f"- strategy mismatch datasets: {strategy_mismatch if strategy_mismatch else 'none'}",
        "",
        "## Validator Output",
        "```",
        resumed_validate_out.strip(),
        resumed_validate_err.strip(),
        full_validate_out.strip(),
        full_validate_err.strip(),
        "```",
        "",
        "## Scenario Digest",
        digest_line,
    ]
    REPORT_PATH.write_text("\n".join(report_lines) + "\n", encoding="utf-8")
    print(f"wrote {REPORT_PATH}")

    if duplicate_dataset_ids or missing_dataset_ids or extra_dataset_ids or strategy_mismatch:
        print("resume comparison failed", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
