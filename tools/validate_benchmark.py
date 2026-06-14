#!/usr/bin/env python3
"""Validate the benchmark CSV emitted by loxsort_bench."""

from __future__ import annotations

import csv
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
    campaign_ids: set[str] = set()
    families: set[str] = set()

    for row in rows:
        dataset_id = row["dataset_id"].strip()
        if not dataset_id:
            print("missing dataset_id", file=sys.stderr)
            return 1
        dataset_rows[dataset_id].append(row)
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
        if row["algorithm"].strip() == "loxsort_dispatcher" and selected_algorithm != "none":
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
        if selected_algorithm != "none":
            regret_value = _parse_float(row["regret"], "regret")
            if regret_value <= 0.0:
                print(f"non-positive regret in dataset {dataset_id}", file=sys.stderr)
                return 1

    for dataset_id, rows_for_dataset in dataset_rows.items():
        if len(rows_for_dataset) != 7:
            print(f"dataset {dataset_id} has {len(rows_for_dataset)} rows, expected 7", file=sys.stderr)
            return 1
        algorithms = [row["algorithm"].strip() for row in rows_for_dataset]
        if set(algorithms) != REQUIRED_STRATEGIES:
            print(f"dataset {dataset_id} has strategies {sorted(set(algorithms))}, expected {sorted(REQUIRED_STRATEGIES)}", file=sys.stderr)
            return 1
        oracle_rows = [row for row in rows_for_dataset if row["algorithm"].strip() == "oracle_best"]
        if len(oracle_rows) != 1:
            print(f"dataset {dataset_id} has invalid oracle row count", file=sys.stderr)
            return 1
        oracle = oracle_rows[0]
        if oracle["regret"].strip() != "1.000" and oracle["regret"].strip() != "1":
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

    merge_eligible_dataset_ids = {
        dataset_id
        for dataset_id, rows_for_dataset in dataset_rows.items()
        if any(
            row["scratch_mode"].strip() in {"exact_merge_scratch", "ample_scratch"}
            for row in rows_for_dataset
        )
    }
    required_merge_strategies = {
        "loxsort_dispatcher",
        "always_shell",
        "always_intro",
        "always_merge_if_available",
        "oracle_best",
    }
    for dataset_id in merge_eligible_dataset_ids:
        rows_for_dataset = dataset_rows[dataset_id]
        strategies = {row["algorithm"].strip() for row in rows_for_dataset}
        if not required_merge_strategies.issubset(strategies):
            print(f"merge-eligible comparison missing strategies for dataset {dataset_id}", file=sys.stderr)
            return 1

    print(f"validated_rows={len(rows)}")
    print(f"validated_datasets={len(dataset_rows)}")
    print(f"campaign_ids={sorted(campaign_ids)}")
    print(f"families={sorted(families)}")
    print(f"merge_eligible_datasets={len(merge_eligible_dataset_ids)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
