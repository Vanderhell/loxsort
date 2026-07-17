#!/usr/bin/env python3
"""Generate a markdown summary from a benchmark CSV."""

from __future__ import annotations

import csv
import datetime as _dt
import hashlib
import math
import os
import platform
import statistics
import subprocess
import sys
from collections import defaultdict
from pathlib import Path


REQUIRED_FAMILIES = [
    "integers",
    "floating_point",
    "characters",
    "strings",
    "records",
    "timestamps",
]

BREAKDOWN_FIELDS = [
    ("data_family", "Data Family"),
    ("data_type", "Data Type"),
    ("pattern", "Pattern"),
    ("count_bucket", "Count Bucket"),
    ("element_size", "Element Size"),
    ("comparator_kind", "Comparator"),
    ("scratch_mode", "Scratch Mode"),
    ("selected_algorithm", "Selected Algorithm"),
]

CANONICAL_TIME_FIELD = "selected_sort_ns"
PUBLIC_API_TIME_FIELD = "public_api_total_ns"


def _safe_float(text: str) -> float:
    value = text.strip()
    if not value or value.upper() == "NA":
        return float("nan")
    return float(value)


def _safe_int(text: str) -> int:
    value = text.strip()
    if not value or value.upper() == "NA":
        return 0
    return int(value)


def _optional_float(text: str) -> float:
    value = text.strip()
    if not value or value.upper() == "NA":
        return float("nan")
    return float(value)


def _fmt_ns(value: float) -> str:
    if value != value:
        return "NA"
    abs_value = abs(value)
    if abs_value < 1000.0:
        return f"{value:.3f} ns"
    if abs_value < 1_000_000.0:
        return f"{value / 1000.0:.3f} us"
    if abs_value < 1_000_000_000.0:
        return f"{value / 1_000_000.0:.3f} ms"
    return f"{value / 1_000_000_000.0:.3f} s"


def _fmt_percent(value: float) -> str:
    if value != value:
        return "NA"
    return f"{value:.3f}%"


def _median(values: list[float]) -> float:
    return statistics.median(values) if values else float("nan")


def _p95(values: list[float]) -> float:
    if not values:
        return float("nan")
    values = sorted(values)
    index = min(len(values) - 1, int((len(values) - 1) * 0.95))
    return values[index]


def _p99(values: list[float]) -> float:
    if not values:
        return float("nan")
    values = sorted(values)
    index = min(len(values) - 1, int((len(values) - 1) * 0.99))
    return values[index]


def _geometric_mean(values: list[float]) -> float:
    positive = [value for value in values if value > 0.0 and value == value]
    if not positive:
        return float("nan")
    return math.exp(sum(math.log(value) for value in positive) / len(positive))


def _count_bucket(count: int) -> str:
    if count <= 3:
        return "0-3"
    if count <= 8:
        return "4-8"
    if count <= 32:
        return "9-32"
    if count <= 128:
        return "33-128"
    if count <= 512:
        return "129-512"
    return "513+"


def _time_bucket(ns: float) -> str:
    if ns < 100.0:
        return "< 100 ns"
    if ns < 1000.0:
        return "100 ns to 1 us"
    if ns < 10_000.0:
        return "1 us to 10 us"
    if ns < 100_000.0:
        return "10 us to 100 us"
    if ns < 1_000_000.0:
        return "100 us to 1 ms"
    return ">= 1 ms"


def _read_rows(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as fh:
        return list(csv.DictReader(fh))


def _is_well_formed_row(row: dict[str, str]) -> bool:
    return all(value is not None for value in row.values())


def _scenario_digest(rows: list[dict[str, str]]) -> str:
    digest = hashlib.sha256()
    keys = []
    for row in rows:
        key = (
            row["data_family"].strip(),
            row["data_type"].strip(),
            row["pattern"].strip(),
            row["count"].strip(),
            row["element_size"].strip(),
            row["comparator_kind"].strip(),
            row["seed"].strip(),
            row["scratch_mode"].strip(),
        )
        keys.append("|".join(key))
    for item in sorted(keys):
        digest.update(item.encode("utf-8"))
        digest.update(b"\n")
    return digest.hexdigest()


def _git_metadata() -> dict[str, str]:
    meta = {
        "commit": "NOT AVAILABLE",
        "dirty": "NOT AVAILABLE",
    }
    try:
        meta["commit"] = subprocess.check_output(["git", "rev-parse", "HEAD"], text=True).strip()
    except Exception:
        pass
    try:
        status = subprocess.check_output(["git", "status", "--short"], text=True).strip()
        meta["dirty"] = "dirty" if status else "clean"
    except Exception:
        pass
    return meta


def _env_metadata(name: str) -> str:
    value = os.environ.get(name, "").strip()
    return value if value else "NOT AVAILABLE"


def _escape_md(text: str) -> str:
    return text.replace("|", "\\|").replace("\n", "<br>").replace("`", "'")


def _dataset_key(row: dict[str, str]) -> tuple[str, ...]:
    return (
        row["data_family"].strip(),
        row["data_type"].strip(),
        row["pattern"].strip(),
        row["count"].strip(),
        row["element_size"].strip(),
        row["comparator_kind"].strip(),
        row["seed"].strip(),
        row["scratch_mode"].strip(),
    )


def _rows_for_strategy(rows: list[dict[str, str]], strategy: str) -> list[dict[str, str]]:
    return [row for row in rows if row["algorithm"].strip() == strategy]


def _numeric_rows(rows: list[dict[str, str]], field: str) -> list[float]:
    values = []
    for row in rows:
        value = _safe_float(row[field])
        if value == value:
            values.append(value)
    return values


def _canonical_time(row: dict[str, str]) -> float | None:
    value = _optional_float(row[CANONICAL_TIME_FIELD])
    if value is None or not math.isfinite(value) or value <= 0.0:
        return None
    return value


def _public_api_time(row: dict[str, str]) -> float | None:
    value = _optional_float(row[PUBLIC_API_TIME_FIELD])
    if value is None or not math.isfinite(value) or value <= 0.0:
        return None
    return value


def _strategy_summary(
    rows: list[dict[str, str]],
    dispatcher_by_dataset: dict[str, dict[str, str]],
) -> dict[str, str]:
    # Strategy comparison uses one canonical metric everywhere:
    # selected-algorithm sort time. Dispatcher public-API totals are reported separately.
    valid_rows = []
    valid_times = []
    for row in rows:
        if row["selected_algorithm"].strip() == "none":
            continue
        time_value = _canonical_time(row)
        if time_value is None:
            continue
        valid_rows.append(row)
        valid_times.append(time_value)
    summary = {
        "total_time": _fmt_ns(sum(valid_times)) if valid_times else "NA",
        "mean_time": _fmt_ns(statistics.mean(valid_times)) if valid_times else "NA",
        "median_time": _fmt_ns(_median(valid_times)) if valid_times else "NA",
        "p95_time": _fmt_ns(_p95(valid_times)) if valid_times else "NA",
        "p99_time": _fmt_ns(_p99(valid_times)) if valid_times else "NA",
        "max_time": _fmt_ns(max(valid_times)) if valid_times else "NA",
        "wins": str(sum(1 for row in valid_rows if row["selected_algorithm"].strip() == row["best_algorithm"].strip() and row["algorithm"].strip() != "oracle_best")),
        "losses": str(sum(1 for row in valid_rows if row["selected_algorithm"].strip() != row["best_algorithm"].strip() and row["algorithm"].strip() != "oracle_best")),
        "ties": str(sum(1 for row in valid_rows if _safe_float(row["regret"]) == 1.0 and row["algorithm"].strip() != "oracle_best")),
    }
    speedup_samples = []
    for row in valid_rows:
        dispatcher_row = dispatcher_by_dataset.get(row["dataset_id"].strip())
        if dispatcher_row is None:
            continue
        dispatcher_time = _canonical_time(dispatcher_row)
        strategy_time = _canonical_time(row)
        if dispatcher_time is not None and strategy_time is not None and dispatcher_time > 0.0 and strategy_time > 0.0:
            speedup_samples.append(dispatcher_time / strategy_time)
    summary["geometric_mean_speedup"] = f"{_geometric_mean(speedup_samples):.3f}x" if speedup_samples else "NA"
    return summary


def _summary_bucket(rows: list[dict[str, str]]) -> dict[str, str]:
    regrets = [_safe_float(row["regret"]) for row in rows if row["regret"].strip()]
    losses = [_safe_float(row["absolute_loss_ns"]) for row in rows if row["absolute_loss_ns"].strip()]
    return {
        "dataset_count": str(len(rows)),
        "p50_regret": f"{_median(regrets):.3f}" if regrets else "NA",
        "p95_regret": f"{_p95(regrets):.3f}" if regrets else "NA",
        "p99_regret": f"{_p99(regrets):.3f}" if regrets else "NA",
        "max_regret": f"{max(regrets):.3f}" if regrets else "NA",
        "median_absolute_loss": _fmt_ns(_median(losses)) if losses else "NA",
        "total_absolute_loss": _fmt_ns(sum(losses)) if losses else "NA",
    }


def _bucket_rows(rows: list[dict[str, str]], bucket_fn) -> dict[str, list[dict[str, str]]]:
    grouped: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in rows:
        grouped[bucket_fn(row)].append(row)
    return grouped


def _merge_eligible_dataset_ids(rows_by_dataset: dict[str, list[dict[str, str]]]) -> set[str]:
    eligible = set()
    for dataset_id, rows in rows_by_dataset.items():
        dispatcher_rows = [row for row in rows if row["algorithm"].strip() == "loxsort_dispatcher"]
        dispatcher_valid = False
        if dispatcher_rows:
            dispatcher_row = dispatcher_rows[0]
            dispatcher_valid = dispatcher_row["selected_algorithm"].strip() != "none" and _canonical_time(dispatcher_row) is not None
        if any(row["scratch_mode"].strip() in {"exact_merge_scratch", "ample_scratch"} for row in rows) and dispatcher_valid:
            eligible.add(dataset_id)
    return eligible


def _dataset_time_lookup(rows: list[dict[str, str]], strategy: str) -> dict[str, float]:
    lookup: dict[str, float] = {}
    for row in rows:
        if row["algorithm"].strip() != strategy:
            continue
        value = _canonical_time(row)
        if value is not None:
            lookup[row["dataset_id"].strip()] = value
    return lookup


def _format_sample_values(rows: list[dict[str, str]], family: str) -> str:
    values = []
    seen = set()
    for row in rows:
        if row["data_family"].strip() != family:
            continue
        sample = row.get("sample_preview", "").strip()
        if not sample or sample in seen:
            continue
        values.append(sample)
        seen.add(sample)
        if len(values) >= 3:
            break
    if not values:
        return "NOT AVAILABLE"
    return " ; ".join(f"`{_escape_md(value)}`" for value in values)


def main(argv: list[str]) -> int:
    if len(argv) != 3:
        print("usage: generate_benchmark_report.py input.csv output.md", file=sys.stderr)
        return 2

    csv_path = Path(argv[1])
    md_path = Path(argv[2])
    rows = _read_rows(csv_path)
    if not rows:
        print("empty benchmark CSV", file=sys.stderr)
        return 1

    malformed_rows = [row for row in rows if not _is_well_formed_row(row)]
    if malformed_rows:
        print(f"warning: skipped {len(malformed_rows)} malformed CSV row(s)", file=sys.stderr)
        rows = [row for row in rows if _is_well_formed_row(row)]
        if not rows:
            print("no valid benchmark rows after filtering malformed input", file=sys.stderr)
            return 1

    dataset_rows: dict[str, list[dict[str, str]]] = defaultdict(list)
    dataset_keys: dict[str, dict[str, str]] = {}
    strategies: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in rows:
        dataset_id = row["dataset_id"].strip()
        strategies[row["algorithm"].strip()].append(row)
        dataset_rows[dataset_id].append(row)
        dataset_keys[dataset_id] = row

    meta = _git_metadata()
    scenario_digest = _scenario_digest(list(dataset_keys.values()))
    dispatcher_rows = strategies.get("loxsort_dispatcher", [])
    dispatcher_by_dataset = {row["dataset_id"].strip(): row for row in dispatcher_rows}
    oracle_by_dataset = _dataset_time_lookup(rows, "oracle_best")
    eligible_dataset_ids = _merge_eligible_dataset_ids(dataset_rows)
    eligible_rows = [row for row in rows if row["dataset_id"].strip() in eligible_dataset_ids]

    lines: list[str] = []
    lines.append("# Benchmark Report")
    lines.append("")
    lines.append("## Metadata")
    lines.append(f"- git_commit: {meta['commit']}")
    lines.append(f"- git_state: {meta['dirty']}")
    lines.append(f"- os: {platform.platform()}")
    lines.append(f"- cpu: {platform.processor() or 'NOT AVAILABLE'}")
    lines.append(f"- compiler: {_env_metadata('LOXSORT_BENCH_COMPILER')}")
    lines.append(f"- compiler_version: {_env_metadata('LOXSORT_BENCH_COMPILER_VERSION')}")
    lines.append(f"- build_configuration: {_env_metadata('LOXSORT_BUILD_CONFIGURATION')}")
    lines.append(f"- optimization_flags: {_env_metadata('LOXSORT_OPT_FLAGS')}")
    lines.append(f"- cmake_options: {_env_metadata('LOXSORT_CMAKE_OPTIONS')}")
    lines.append(f"- clock_source: {_env_metadata('LOXSORT_CLOCK_SOURCE')}")
    clock_frequency = next((row["clock_frequency_hz"].strip() for row in dispatcher_rows if row["clock_frequency_hz"].strip()), "NOT AVAILABLE")
    lines.append(f"- clock_frequency_hz: {clock_frequency if clock_frequency else 'NOT AVAILABLE'}")
    lines.append(f"- campaign_command: {_env_metadata('LOXSORT_BENCH_COMMAND')}")
    lines.append(f"- run_timestamp: {_dt.datetime.now().astimezone().isoformat()}")
    lines.append(f"- csv_path: {csv_path}")
    lines.append(f"- scenario_digest: {scenario_digest}")
    lines.append(f"- planned_dataset_count: {len(dataset_rows)}")
    lines.append(f"- executed_dataset_count: {len(dataset_rows)}")
    lines.append(f"- row_count: {len(rows)}")
    lines.append("")

    lines.append("## Overall Strategy Results")
    lines.append("")
    lines.append("| strategy | total_time | mean_time | median_time | geometric_mean_speedup | p95_time | p99_time | max_time | wins | losses | ties |")
    lines.append("| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    for strategy in [
        "always_insertion",
        "always_shell",
        "always_intro",
        "always_merge_if_available",
        "always_cycle_if_available",
        "loxsort_dispatcher",
        "oracle_best",
    ]:
        summary = _strategy_summary(strategies.get(strategy, []), dispatcher_by_dataset)
        lines.append(
            f"| {strategy} | {summary['total_time']} | {summary['mean_time']} | {summary['median_time']} | {summary['geometric_mean_speedup']} | {summary['p95_time']} | {summary['p99_time']} | {summary['max_time']} | {summary['wins']} | {summary['losses']} | {summary['ties']} |"
        )

    lines.append("")
    lines.append("## LoxSort Quality")
    lines.append("")
    valid_dispatcher_rows = []
    selected_algorithm_times = []
    public_api_times = []
    dispatcher_overhead_times = []
    regret_values = []
    loss_values = []
    overhead_values = []
    for row in dispatcher_rows:
        if row["selected_algorithm"].strip() == "none":
            continue
        selected_time = _canonical_time(row)
        if selected_time is None:
            continue
        valid_dispatcher_rows.append(row)
        selected_algorithm_times.append(selected_time)
        public_api_time = _public_api_time(row)
        if public_api_time is not None:
            public_api_times.append(public_api_time)
        overhead_time = _safe_float(row["dispatcher_overhead_ns"])
        if overhead_time == overhead_time:
            dispatcher_overhead_times.append(overhead_time)
        regret_value = _safe_float(row["regret"])
        if regret_value == regret_value:
            regret_values.append(regret_value)
        loss_value = _safe_float(row["absolute_loss_ns"])
        if loss_value == loss_value:
            loss_values.append(loss_value)
        overhead_percent = _safe_float(row["dispatcher_overhead_percent"])
        if overhead_percent == overhead_percent:
            overhead_values.append(overhead_percent)
    if regret_values:
        lines.append(f"- selected_algorithm_total: {_fmt_ns(sum(selected_algorithm_times)) if selected_algorithm_times else 'NA'}")
        lines.append(f"- public_api_total: {_fmt_ns(sum(public_api_times)) if public_api_times else 'NA'}")
        lines.append(f"- dispatcher_overhead_total: {_fmt_ns(sum(dispatcher_overhead_times)) if dispatcher_overhead_times else 'NA'}")
        lines.append(f"- p50_regret: {_median(regret_values):.3f}")
        lines.append(f"- p95_regret: {_p95(regret_values):.3f}")
        lines.append(f"- p99_regret: {_p99(regret_values):.3f}")
        lines.append(f"- max_regret: {max(regret_values):.3f}")
        lines.append(f"- mean_absolute_loss: {_fmt_ns(statistics.mean(loss_values))}" if loss_values else "- mean_absolute_loss: NA")
        lines.append(f"- median_absolute_loss: {_fmt_ns(_median(loss_values))}" if loss_values else "- median_absolute_loss: NA")
        lines.append(f"- total_absolute_loss: {_fmt_ns(sum(loss_values))}" if loss_values else "- total_absolute_loss: NA")
        lines.append(f"- dispatcher_overhead_median: {_fmt_ns(_median(overhead_values))}" if overhead_values else "- dispatcher_overhead_median: NA")
        lines.append(f"- dispatcher_overhead_p95: {_fmt_ns(_p95(overhead_values))}" if overhead_values else "- dispatcher_overhead_p95: NA")
    else:
        lines.append("- selected_algorithm_total: NA")
        lines.append("- public_api_total: NA")
        lines.append("- dispatcher_overhead_total: NA")
        lines.append("- p50_regret: NA")
        lines.append("- p95_regret: NA")
        lines.append("- p99_regret: NA")
        lines.append("- max_regret: NA")
        lines.append("- mean_absolute_loss: NA")
        lines.append("- median_absolute_loss: NA")
        lines.append("- total_absolute_loss: NA")
        lines.append("- dispatcher_overhead_median: NA")
        lines.append("- dispatcher_overhead_p95: NA")

    lines.append("")
    lines.append("## Merge-Eligible Results")
    lines.append("")
    lines.append(f"- eligible_dataset_count: {len(eligible_dataset_ids)}")
    lines.append("")
    lines.append("| strategy | total_time | mean_time | median_time | p95_time | wins | losses | ties | speedup_vs_dispatcher |")
    lines.append("| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    eligible_strategies = [
        "loxsort_dispatcher",
        "always_shell",
        "always_intro",
        "always_merge_if_available",
        "oracle_best",
    ]
    eligible_dispatcher_time = sum(row_time for row in valid_dispatcher_rows if row["dataset_id"].strip() in eligible_dataset_ids for row_time in [_canonical_time(row)] if row_time is not None)
    for strategy in eligible_strategies:
        subset_rows = [row for row in strategies.get(strategy, []) if row["dataset_id"].strip() in eligible_dataset_ids]
        summary = _strategy_summary(subset_rows, dispatcher_by_dataset)
        subset_total = sum(row_time for row in subset_rows for row_time in [_canonical_time(row)] if row_time is not None)
        if strategy == "loxsort_dispatcher" or subset_total <= 0.0 or eligible_dispatcher_time <= 0.0:
            speedup = "1.000x" if strategy == "loxsort_dispatcher" else "NA"
        else:
            speedup = f"{eligible_dispatcher_time / subset_total:.3f}x"
        lines.append(
            f"| {strategy} | {summary['total_time']} | {summary['mean_time']} | {summary['median_time']} | {summary['p95_time']} | {summary['wins']} | {summary['losses']} | {summary['ties']} | {speedup} |"
        )

    lines.append("")
    lines.append("## Coverage")
    lines.append("")
    lines.append("| family | data types | comparators | dataset count | count range | element-size range | patterns used | sample values |")
    lines.append("| --- | --- | --- | ---: | --- | --- | --- | --- |")
    for family in REQUIRED_FAMILIES:
        family_rows = [row for row in rows if row["data_family"].strip() == family]
        family_dataset_ids = {row["dataset_id"].strip() for row in family_rows}
        counts = [_safe_int(row["count"]) for row in family_rows]
        element_sizes = [_safe_int(row["element_size"]) for row in family_rows]
        data_types = sorted({row["data_type"].strip() for row in family_rows})
        comparators = sorted({row["comparator_kind"].strip() for row in family_rows})
        patterns = sorted({row["pattern"].strip() for row in family_rows})
        lines.append(
            "| {family} | {types} | {comparators} | {dataset_count} | {count_range} | {esize_range} | {patterns} | {samples} |".format(
                family=family,
                types=", ".join(data_types) if data_types else "NOT AVAILABLE",
                comparators=", ".join(comparators) if comparators else "NOT AVAILABLE",
                dataset_count=len(family_dataset_ids),
                count_range=f"{min(counts)}-{max(counts)}" if counts else "NOT AVAILABLE",
                esize_range=f"{min(element_sizes)}-{max(element_sizes)}" if element_sizes else "NOT AVAILABLE",
                patterns=", ".join(patterns) if patterns else "NOT AVAILABLE",
                samples=_format_sample_values(family_rows, family),
            )
        )

    lines.append("")
    lines.append("## Regret Buckets")
    lines.append("")
    lines.append("### Oracle Absolute Time")
    lines.append("")
    lines.append("| bucket | dataset count | p50 regret | p95 regret | p99 regret | max regret | median absolute loss | total absolute loss |")
    lines.append("| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    oracle_bucket_groups: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in valid_dispatcher_rows:
        oracle_time = oracle_by_dataset.get(row["dataset_id"].strip(), float("nan"))
        oracle_bucket_groups[_time_bucket(oracle_time)].append(row)
    for bucket in ["< 100 ns", "100 ns to 1 us", "1 us to 10 us", "10 us to 100 us", "100 us to 1 ms", ">= 1 ms"]:
        summary = _summary_bucket(oracle_bucket_groups.get(bucket, []))
        lines.append(
            f"| {bucket} | {summary['dataset_count']} | {summary['p50_regret']} | {summary['p95_regret']} | {summary['p99_regret']} | {summary['max_regret']} | {summary['median_absolute_loss']} | {summary['total_absolute_loss']} |"
        )

    lines.append("")
    lines.append("### Count")
    lines.append("")
    lines.append("| bucket | dataset count | p50 regret | p95 regret | p99 regret | max regret | median absolute loss | total absolute loss |")
    lines.append("| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    count_bucket_groups = _bucket_rows(valid_dispatcher_rows, lambda row: _count_bucket(_safe_int(row["count"])))
    for bucket in ["0-3", "4-8", "9-32", "33-128", "129-512", "513+"]:
        summary = _summary_bucket(count_bucket_groups.get(bucket, []))
        lines.append(
            f"| {bucket} | {summary['dataset_count']} | {summary['p50_regret']} | {summary['p95_regret']} | {summary['p99_regret']} | {summary['max_regret']} | {summary['median_absolute_loss']} | {summary['total_absolute_loss']} |"
        )

    lines.append("")
    lines.append("## Worst Cases")
    lines.append("")
    worst_tables = [
        ("regret", "regret"),
        ("absolute_loss_ns", "absolute_loss_ns"),
        ("dispatcher_overhead_percent", "dispatcher_overhead_percent"),
    ]
    for field, title in worst_tables:
        lines.append(f"### Top 100 by {title}")
        lines.append("")
        lines.append("| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |")
        lines.append("| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |")
        worst = sorted(
            valid_dispatcher_rows,
            key=lambda row: (_safe_float(row[field]), _safe_float(row["absolute_loss_ns"])),
            reverse=True,
        )[:100]
        for row in worst:
            selected_time = _canonical_time(row)
            regret_value = _safe_float(row["regret"])
            best_time = selected_time / regret_value if selected_time == selected_time and regret_value > 0.0 else float("nan")
            features = (
                f"pairs={row['sampled_pair_count']}, disorder={row['disorder_score']}, equal_score={row['equal_score']}, "
                f"equal_pairs={row['equal_pair_count']}, dir_changes={row['direction_changes']}, sign_mask={row['comparison_sign_mask']}, "
                f"fallback={row['profile_fallback_used']}, reason={row['decision_reason']}"
            )
            lines.append(
                "| {dataset_id} | {family} | {dtype} | {pattern} | {count} | {esize} | {seed} | {sel} | {best} | {sel_time} | {best_time} | {regret} | {loss} | {features} |".format(
                    dataset_id=row["dataset_id"],
                    family=row["data_family"],
                    dtype=row["data_type"],
                    pattern=row["pattern"],
                    count=row["count"],
                    esize=row["element_size"],
                    seed=row["seed"],
                    sel=row["selected_algorithm"],
                    best=row["best_algorithm"],
                    sel_time=_fmt_ns(selected_time),
                    best_time=_fmt_ns(best_time),
                    regret=row["regret"],
                    loss=_fmt_ns(_safe_float(row["absolute_loss_ns"])),
                    features=_escape_md(features),
                )
            )
        lines.append("")

    md_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"wrote {md_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
