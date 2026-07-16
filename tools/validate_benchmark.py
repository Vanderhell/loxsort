#!/usr/bin/env python3
"""Validate the benchmark CSV emitted by loxsort_bench."""

from __future__ import annotations

import csv
import math
import sys
from collections import defaultdict
from pathlib import Path


REQUIRED_COLUMNS = {
    "campaign_id",
    "dataset_id",
    "data_family",
    "data_type",
    "pattern",
    "count",
    "element_size",
    "data_bytes",
    "comparator_kind",
    "seed",
    "scratch_mode",
    "algorithm",
    "selected_algorithm",
    "best_algorithm",
    "clock_frequency_hz",
    "timing_iterations",
    "sample_count",
    "elapsed_ticks_total",
    "ticks_per_iteration",
    "min_ns",
    "median_ns",
    "mean_ns",
    "p95_ns",
    "max_ns",
    "feature_extraction_ns",
    "dispatch_only_ns",
    "selected_sort_ns",
    "public_api_total_ns",
    "dispatcher_overhead_ns",
    "dispatcher_overhead_percent",
    "comparison_count",
    "move_count",
    "swap_count",
    "bytes_moved",
    "sorted_ok",
    "multiset_ok",
    "payload_ok",
    "canary_ok",
    "decision_reason",
    "profile_fallback_used",
    "sampled_pair_count",
    "disorder_score",
    "equal_score",
    "equal_pair_count",
    "direction_changes",
    "comparison_sign_mask",
    "regret",
    "absolute_loss_ns",
    "sample_preview",
}

REQUIRED_STRATEGIES = {
    "always_insertion",
    "always_shell",
    "always_intro",
    "always_merge_if_available",
    "always_cycle_if_available",
    "loxsort_dispatcher",
    "oracle_best",
}

CANONICAL_TIME_FIELD = "selected_sort_ns"
PUBLIC_API_TIME_FIELD = "public_api_total_ns"
TIMING_TOLERANCE_NS = 1e-3
MERGE_ELIGIBLE_SCRATCH_MODES = {
    "exact_merge_scratch",
    "ample_scratch",
}
MERGE_COMPARISON_STRATEGIES = {
    "loxsort_dispatcher",
    "always_shell",
    "always_intro",
    "always_merge_if_available",
    "oracle_best",
}
CONCRETE_STRATEGIES = {
    "always_insertion",
    "always_shell",
    "always_intro",
    "always_merge_if_available",
    "always_cycle_if_available",
}


def _parse_int(text: str, field: str) -> int:
    value = text.strip()
    if not value:
        raise ValueError(f"empty integer field {field}")
    return int(value)


def _parse_float(text: str, field: str) -> float:
    value = text.strip()
    if not value:
        raise ValueError(f"empty float field {field}")
    return float(value)


def _parse_optional_float(text: str) -> float | None:
    value = text.strip()
    if not value or value.upper() == "NA":
        return None
    parsed = float(value)
    if not math.isfinite(parsed):
        return None
    return parsed


def _canonical_time(row: dict[str, str]) -> float | None:
    value = _parse_optional_float(row[CANONICAL_TIME_FIELD])
    if value is None or value <= 0.0:
        return None
    return value


def _public_api_time(row: dict[str, str]) -> float | None:
    value = _parse_optional_float(row[PUBLIC_API_TIME_FIELD])
    if value is None or value <= 0.0:
        return None
    return value


def _row_error(dataset_id: str, strategy: str, oracle_algorithm: str, oracle_time: float | None, compared_time: float | None, timing_field: str, message: str) -> str:
    oracle_text = "NA" if oracle_time is None else f"{oracle_time:.3f}"
    compared_text = "NA" if compared_time is None else f"{compared_time:.3f}"
    return (
        f"dataset_id={dataset_id} strategy={strategy} oracle_algorithm={oracle_algorithm} "
        f"oracle_time={oracle_text} compared_time={compared_text} timing_field={timing_field}: {message}"
    )


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: validate_benchmark.py benchmark.csv", file=sys.stderr)
        return 2

    path = Path(argv[1])
    with path.open("r", encoding="utf-8-sig", newline="") as fh:
        rows = list(csv.DictReader(fh))

    if not rows:
        print("empty benchmark CSV", file=sys.stderr)
        return 1

    missing = REQUIRED_COLUMNS.difference(rows[0].keys())
    if missing:
        print(f"missing required columns: {sorted(missing)}", file=sys.stderr)
        return 1

    dataset_rows: defaultdict[str, list[dict[str, str]]] = defaultdict(list)
    dataset_strategy_rows: defaultdict[str, dict[str, dict[str, str]]] = defaultdict(dict)
    campaign_ids: set[str] = set()
    families: set[str] = set()

    for row in rows:
        dataset_id = row["dataset_id"].strip()
        strategy = row["algorithm"].strip()
        if not dataset_id:
            print("missing dataset_id", file=sys.stderr)
            return 1
        if strategy in dataset_strategy_rows[dataset_id]:
            print(f"duplicate strategy row for dataset {dataset_id}: {strategy}", file=sys.stderr)
            return 1
        dataset_rows[dataset_id].append(row)
        dataset_strategy_rows[dataset_id][strategy] = row
        campaign_ids.add(row["campaign_id"].strip())
        families.add(row["data_family"].strip())

        try:
            _parse_int(row["count"], "count")
            _parse_int(row["element_size"], "element_size")
            _parse_int(row["data_bytes"], "data_bytes")
            _parse_int(row["seed"], "seed")
            _parse_int(row["clock_frequency_hz"], "clock_frequency_hz")
            timing_iterations = _parse_int(row["timing_iterations"], "timing_iterations")
            sample_count = _parse_int(row["sample_count"], "sample_count")
            elapsed_ticks_total = _parse_int(row["elapsed_ticks_total"], "elapsed_ticks_total")
            ticks_per_iteration = _parse_int(row["ticks_per_iteration"], "ticks_per_iteration")
            sorted_ok = _parse_int(row["sorted_ok"], "sorted_ok")
            multiset_ok = _parse_int(row["multiset_ok"], "multiset_ok")
            payload_ok = _parse_int(row["payload_ok"], "payload_ok")
            canary_ok = _parse_int(row["canary_ok"], "canary_ok")
            _parse_int(row["decision_reason"], "decision_reason")
            _parse_int(row["profile_fallback_used"], "profile_fallback_used")
            _parse_int(row["sampled_pair_count"], "sampled_pair_count")
            _parse_int(row["disorder_score"], "disorder_score")
            _parse_int(row["equal_score"], "equal_score")
            _parse_int(row["equal_pair_count"], "equal_pair_count")
            _parse_int(row["direction_changes"], "direction_changes")
            _parse_int(row["comparison_sign_mask"], "comparison_sign_mask")
            regret = _parse_float(row["regret"], "regret")
            absolute_loss_ns = _parse_float(row["absolute_loss_ns"], "absolute_loss_ns")
            sample_preview = row["sample_preview"].strip()
            _parse_float(row["median_ns"], "median_ns")
            _parse_float(row["mean_ns"], "mean_ns")
            _parse_float(row["p95_ns"], "p95_ns")
            _parse_float(row["max_ns"], "max_ns")
            _parse_float(row["selected_sort_ns"], "selected_sort_ns")
            _parse_float(row["public_api_total_ns"], "public_api_total_ns")
            _parse_float(row["dispatcher_overhead_ns"], "dispatcher_overhead_ns")
            _parse_float(row["dispatcher_overhead_percent"], "dispatcher_overhead_percent")
        except ValueError as exc:
            print(f"invalid numeric value in dataset {dataset_id}: {exc}", file=sys.stderr)
            return 1

        selected_algorithm = row["selected_algorithm"].strip()
        canonical_time = _canonical_time(row)
        public_api_time = _public_api_time(row)
        unavailable_row = selected_algorithm == "none"
        if not unavailable_row:
            if timing_iterations <= 0:
                print(f"non-positive timing_iterations in dataset {dataset_id}", file=sys.stderr)
                return 1
            if sample_count <= 0:
                print(f"non-positive sample_count in dataset {dataset_id}", file=sys.stderr)
                return 1
            if elapsed_ticks_total < 0 or ticks_per_iteration < 0:
                print(f"negative timing fields in dataset {dataset_id}", file=sys.stderr)
                return 1
            if canonical_time is None:
                print(
                    _row_error(
                        dataset_id,
                        strategy,
                        row["best_algorithm"].strip(),
                        canonical_time,
                        None,
                        CANONICAL_TIME_FIELD,
                        "missing canonical selected-algorithm timing",
                    ),
                    file=sys.stderr,
                )
                return 1
        if sorted_ok not in (0, 1) or multiset_ok not in (0, 1) or payload_ok not in (0, 1) or canary_ok not in (0, 1):
            print(f"invalid correctness flags in dataset {dataset_id}", file=sys.stderr)
            return 1
        if regret < 0.0 or absolute_loss_ns < 0.0:
            print(f"negative regret or loss in dataset {dataset_id}", file=sys.stderr)
            return 1
        if _parse_int(row["data_bytes"], "data_bytes") != _parse_int(row["count"], "count") * _parse_int(row["element_size"], "element_size"):
            print(f"invalid data_bytes in dataset {dataset_id}", file=sys.stderr)
            return 1
        if not sample_preview:
            print(f"missing sample preview in dataset {dataset_id}", file=sys.stderr)
            return 1
        if strategy == "loxsort_dispatcher":
            if selected_algorithm != "none":
                if canonical_time is None or public_api_time is None:
                    print(
                        _row_error(
                            dataset_id,
                            strategy,
                            row["best_algorithm"].strip(),
                            canonical_time,
                            public_api_time,
                            CANONICAL_TIME_FIELD,
                            "dispatcher row missing canonical timing fields",
                        ),
                        file=sys.stderr,
                    )
                    return 1
                zero_fields = (
                    row["feature_extraction_ns"].strip() in {"0", "0.0", "0.000"}
                    and row["dispatch_only_ns"].strip() in {"0", "0.0", "0.000"}
                    and row["selected_sort_ns"].strip() in {"0", "0.0", "0.000"}
                    and row["public_api_total_ns"].strip() in {"0", "0.0", "0.000"}
                    and row["dispatcher_overhead_ns"].strip() in {"0", "0.0", "0.000"}
                )
                if zero_fields:
                    print(f"fake zero dispatcher overhead in dataset {dataset_id}", file=sys.stderr)
                    return 1
        else:
            if strategy != "oracle_best" and selected_algorithm != "none" and public_api_time is not None:
                print(
                    _row_error(
                        dataset_id,
                        strategy,
                        row["best_algorithm"].strip(),
                        canonical_time,
                        public_api_time,
                        PUBLIC_API_TIME_FIELD,
                        "incompatible timing scope: fixed strategy must not use public API total",
                    ),
                    file=sys.stderr,
                )
                return 1
        if selected_algorithm != "none":
            regret_value = _parse_float(row["regret"], "regret")
            if regret_value <= 0.0:
                print(f"non-positive regret in dataset {dataset_id}", file=sys.stderr)
                return 1

    for dataset_id, rows_for_dataset in dataset_rows.items():
        strategies_for_dataset = dataset_strategy_rows[dataset_id]
        if len(rows_for_dataset) != 7:
            print(f"dataset {dataset_id} has {len(rows_for_dataset)} rows, expected 7", file=sys.stderr)
            return 1
        if set(strategies_for_dataset.keys()) != REQUIRED_STRATEGIES:
            print(
                f"dataset {dataset_id} has strategies {sorted(set(strategies_for_dataset.keys()))}, expected {sorted(REQUIRED_STRATEGIES)}",
                file=sys.stderr,
            )
            return 1
        oracle_row = strategies_for_dataset["oracle_best"]
        oracle_time = _canonical_time(oracle_row)
        if oracle_row["selected_algorithm"].strip() == "none" or oracle_time is None:
            print(
                _row_error(
                    dataset_id,
                    "oracle_best",
                    oracle_row["best_algorithm"].strip(),
                    oracle_time,
                    None,
                    CANONICAL_TIME_FIELD,
                    "oracle row is not a valid canonical timing row",
                ),
                file=sys.stderr,
            )
            return 1
        oracle_regret = _parse_float(oracle_row["regret"], "regret")
        if oracle_regret != 1.0 and oracle_regret != 1:
            print(f"dataset {dataset_id} has invalid oracle regret", file=sys.stderr)
            return 1

    expected_families = {
        "integers",
        "floating_point",
        "characters",
        "strings",
        "records",
        "timestamps",
    }
    if not expected_families.issubset(families):
        print(f"missing family coverage: {sorted(expected_families.difference(families))}", file=sys.stderr)
        return 1

    merge_eligible_dataset_ids = sorted(
        dataset_id
        for dataset_id, rows_for_dataset in dataset_rows.items()
        if any(row["scratch_mode"].strip() in MERGE_ELIGIBLE_SCRATCH_MODES for row in rows_for_dataset)
        and _canonical_time(dataset_strategy_rows[dataset_id].get("loxsort_dispatcher", {})) is not None
        and dataset_strategy_rows[dataset_id].get("loxsort_dispatcher", {}).get("selected_algorithm", "").strip() != "none"
    )
    comparison_time_by_strategy: dict[str, dict[str, float]] = {
        strategy: {} for strategy in MERGE_COMPARISON_STRATEGIES.union(CONCRETE_STRATEGIES)
    }
    oracle_row_by_dataset: dict[str, dict[str, str]] = {}

    for dataset_id in merge_eligible_dataset_ids:
        strategies_for_dataset = dataset_strategy_rows[dataset_id]
        oracle_row = strategies_for_dataset["oracle_best"]
        oracle_time = _canonical_time(oracle_row)
        oracle_algorithm = oracle_row["selected_algorithm"].strip()
        if oracle_time is None or oracle_algorithm == "none":
            print(
                _row_error(
                    dataset_id,
                    "oracle_best",
                    oracle_row["best_algorithm"].strip(),
                    oracle_time,
                    None,
                    CANONICAL_TIME_FIELD,
                    "oracle row is unavailable for a merge-eligible dataset",
                ),
                file=sys.stderr,
            )
            return 1
        oracle_row_by_dataset[dataset_id] = oracle_row
        comparison_time_by_strategy["oracle_best"][dataset_id] = oracle_time

        for strategy in MERGE_COMPARISON_STRATEGIES:
            row = strategies_for_dataset.get(strategy)
            if row is None:
                print(
                    _row_error(
                        dataset_id,
                        strategy,
                        oracle_algorithm,
                        oracle_time,
                        None,
                        CANONICAL_TIME_FIELD,
                        "missing merge-eligible strategy row",
                    ),
                    file=sys.stderr,
                )
                return 1
            compared_time = _canonical_time(row)
            if compared_time is None or row["selected_algorithm"].strip() == "none":
                print(
                    _row_error(
                        dataset_id,
                        strategy,
                        oracle_algorithm,
                        oracle_time,
                        compared_time,
                        CANONICAL_TIME_FIELD,
                        "merge-eligible strategy row has no canonical timing",
                    ),
                    file=sys.stderr,
                )
                return 1
            comparison_time_by_strategy[strategy][dataset_id] = compared_time

        for strategy in CONCRETE_STRATEGIES:
            row = strategies_for_dataset.get(strategy)
            if row is None:
                continue
            compared_time = _canonical_time(row)
            if row["selected_algorithm"].strip() == "none" or compared_time is None:
                continue
            comparison_time_by_strategy[strategy][dataset_id] = compared_time
            if oracle_time > compared_time + TIMING_TOLERANCE_NS:
                print(
                    _row_error(
                        dataset_id,
                        strategy,
                        oracle_algorithm,
                        oracle_time,
                        compared_time,
                        CANONICAL_TIME_FIELD,
                        "oracle time exceeds concrete algorithm time",
                    ),
                    file=sys.stderr,
                )
                return 1

        dispatcher_time = comparison_time_by_strategy["loxsort_dispatcher"].get(dataset_id)
        if dispatcher_time is None:
            print(
                _row_error(
                    dataset_id,
                    "loxsort_dispatcher",
                    oracle_algorithm,
                    oracle_time,
                    None,
                    CANONICAL_TIME_FIELD,
                    "missing dispatcher timing for merge-eligible dataset",
                ),
                file=sys.stderr,
            )
            return 1
        if oracle_time > dispatcher_time + TIMING_TOLERANCE_NS:
            print(
                _row_error(
                    dataset_id,
                    "loxsort_dispatcher",
                    oracle_algorithm,
                    oracle_time,
                    dispatcher_time,
                    CANONICAL_TIME_FIELD,
                    "oracle time exceeds dispatcher selected-algorithm time",
                ),
                file=sys.stderr,
            )
            return 1

    reference_dataset_ids = sorted(comparison_time_by_strategy["loxsort_dispatcher"].keys(), key=int)
    for strategy in MERGE_COMPARISON_STRATEGIES:
        dataset_ids = sorted(comparison_time_by_strategy[strategy].keys(), key=int)
        if dataset_ids != reference_dataset_ids:
            missing = [dataset_id for dataset_id in reference_dataset_ids if dataset_id not in comparison_time_by_strategy[strategy]]
            extra = [dataset_id for dataset_id in dataset_ids if dataset_id not in comparison_time_by_strategy["loxsort_dispatcher"]]
            dataset_id = missing[0] if missing else (extra[0] if extra else (reference_dataset_ids[0] if reference_dataset_ids else "aggregate"))
            oracle_row = oracle_row_by_dataset.get(dataset_id, next(iter(oracle_row_by_dataset.values())))
            oracle_algorithm = oracle_row["selected_algorithm"].strip() if oracle_row else "none"
            oracle_time = _canonical_time(oracle_row) if oracle_row else None
            compared_time = comparison_time_by_strategy[strategy].get(dataset_id)
            print(
                _row_error(
                    dataset_id,
                    strategy,
                    oracle_algorithm,
                    oracle_time,
                    compared_time,
                    CANONICAL_TIME_FIELD,
                    f"merge-eligible dataset set mismatch missing={missing} extra={extra}",
                ),
                file=sys.stderr,
            )
            return 1

    for strategy in MERGE_COMPARISON_STRATEGIES:
        total = sum(comparison_time_by_strategy[strategy].values())
        comparison_time_by_strategy[strategy]["__total__"] = total
    oracle_total = comparison_time_by_strategy["oracle_best"]["__total__"]
    for strategy in ("loxsort_dispatcher", "always_shell", "always_intro", "always_merge_if_available"):
        compared_total = comparison_time_by_strategy[strategy]["__total__"]
        if oracle_total > compared_total + TIMING_TOLERANCE_NS:
            dataset_id = reference_dataset_ids[0] if reference_dataset_ids else "aggregate"
            oracle_row = oracle_row_by_dataset.get(dataset_id, next(iter(oracle_row_by_dataset.values())))
            oracle_algorithm = oracle_row["selected_algorithm"].strip() if oracle_row else "none"
            oracle_time = _canonical_time(oracle_row) if oracle_row else None
            print(
                _row_error(
                    dataset_id,
                    strategy,
                    oracle_algorithm,
                    oracle_time,
                    compared_total,
                    CANONICAL_TIME_FIELD,
                    "oracle total exceeds compared strategy total",
                ),
                file=sys.stderr,
            )
            return 1

    print(f"validated_rows={len(rows)}")
    print(f"validated_datasets={len(dataset_rows)}")
    print(f"campaign_ids={sorted(campaign_ids)}")
    print(f"families={sorted(families)}")
    print(f"merge_eligible_datasets={len(merge_eligible_dataset_ids)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
