#!/usr/bin/env python3
"""Deterministic profile generator for LoxSort."""

from __future__ import annotations

import csv
import hashlib
import statistics
import sys
from collections import defaultdict
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


def _load_rows(path: Path):
    with path.open("r", newline="", encoding="utf-8") as fh:
        rows = list(csv.DictReader(fh))
    if not rows:
        raise ValueError("empty benchmark summary")
    missing = REQUIRED_COLUMNS.difference(rows[0].keys())
    if missing:
        raise ValueError(f"missing columns: {sorted(missing)}")
    return rows


def _median(values):
    return int(statistics.median(values))


def _generic_profile():
    return {
        "format_version": 1,
        "algorithm_mask": 31,
        "small_count_cutoff": 16,
        "stable_insertion_cutoff": 32,
        "near_sorted_insertion_max_count": 64,
        "cycle_min_count": 64,
        "shell_max_count_elem_small": 256,
        "shell_max_count_elem_medium": 512,
        "shell_max_count_elem_large": 1024,
        "merge_min_count_elem_small": 0,
        "merge_min_count_elem_medium": 0,
        "merge_min_count_elem_large": 0,
        "sample_pairs": 4,
        "near_sorted_disorder_max": 32,
        "cycle_equal_score_max": 32,
        "reserved0": 0,
    }


def _scenario_key(row):
    return row["pattern"], int(row["count"]), int(row["element_size"])


def _algorithm_class(element_size):
    if element_size <= 4:
        return "small"
    if element_size <= 16:
        return "medium"
    return "large"


def _select_algorithm(profile, scenario, medians):
    best_algorithm = None
    best_time = None

    for algorithm, elapsed in medians.items():
        if best_time is None or elapsed < best_time or (elapsed == best_time and algorithm < best_algorithm):
            best_algorithm = algorithm
            best_time = elapsed
    return best_algorithm


def _write_header(profile, out_path: Path, digest: str):
    lines = [
        "#ifndef LOXSORT_GENERATED_PROFILE_H",
        "#define LOXSORT_GENERATED_PROFILE_H",
        '#include "loxsort/loxsort.h"',
        "",
        f"/* input_sha256: {digest} */",
        "static const lox_profile_t lox_profile_target = {",
        f"    {profile['format_version']}u,",
        f"    {profile['algorithm_mask']}u,",
        f"    {profile['small_count_cutoff']}u,",
        f"    {profile['stable_insertion_cutoff']}u,",
        f"    {profile['near_sorted_insertion_max_count']}u,",
        f"    {profile['cycle_min_count']}u,",
        f"    {profile['shell_max_count_elem_small']}u,",
        f"    {profile['shell_max_count_elem_medium']}u,",
        f"    {profile['shell_max_count_elem_large']}u,",
        f"    {profile['merge_min_count_elem_small']}u,",
        f"    {profile['merge_min_count_elem_medium']}u,",
        f"    {profile['merge_min_count_elem_large']}u,",
        f"    {profile['sample_pairs']}u,",
        f"    {profile['near_sorted_disorder_max']}u,",
        f"    {profile['cycle_equal_score_max']}u,",
        f"    {profile['reserved0']}u",
        "};",
        "",
        "#endif",
        "",
    ]
    out_path.write_text("\n".join(lines), encoding="utf-8")


def _write_report(rows, profile, out_path: Path, regrets_by_scenario):
    lines = [
        "# LoxSort profile report",
        "",
        "## Selected profile",
    ]
    for key in [
        "format_version",
        "algorithm_mask",
        "small_count_cutoff",
        "stable_insertion_cutoff",
        "near_sorted_insertion_max_count",
        "cycle_min_count",
        "shell_max_count_elem_small",
        "shell_max_count_elem_medium",
        "shell_max_count_elem_large",
        "merge_min_count_elem_small",
        "merge_min_count_elem_medium",
        "merge_min_count_elem_large",
        "sample_pairs",
        "near_sorted_disorder_max",
        "cycle_equal_score_max",
    ]:
        lines.append(f"- {key}: {profile[key]}")
    lines.extend(["", "## Regret summary"])
    for scenario, values in sorted(regrets_by_scenario.items()):
        best = min(values.values())
        worst = max(values.values())
        lines.append(f"- {scenario}: best={best:.3f} worst={worst:.3f}")
    out_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main(argv: list[str]) -> int:
    if len(argv) != 4:
        print("usage: generate_profile.py input.csv output.h output.md", file=sys.stderr)
        return 2

    input_csv = Path(argv[1])
    output_h = Path(argv[2])
    output_md = Path(argv[3])
    rows = _load_rows(input_csv)
    digest = hashlib.sha256(input_csv.read_bytes()).hexdigest()

    by_scenario = defaultdict(lambda: defaultdict(list))
    for row in rows:
        if row["correctness"].strip().lower() != "ok":
            raise ValueError(f"incorrect row for {row['algorithm']} {row['pattern']}")
        scenario = _scenario_key(row)
        by_scenario[scenario][row["algorithm"]].append(int(row["elapsed_ticks"]))

    scenario_medians = {}
    selected_regrets = {}
    insertion_cutoff = None
    near_sorted_cutoff = 0
    merge_thresholds = {"small": 0, "medium": 0, "large": 0}
    shell_limits = {"small": 0, "medium": 0, "large": 0}
    cycle_min_count = 0

    for scenario, algorithms in by_scenario.items():
        medians = {algorithm: _median(times) for algorithm, times in algorithms.items()}
        scenario_medians[scenario] = medians
        best = max(1, min(medians.values()))
        pattern, count, element_size = scenario
        size_class = _algorithm_class(element_size)
        if insertion_cutoff is None and medians.get("insertion", best) > best:
            insertion_cutoff = count
        insertion_regret = max(1, medians.get("insertion", best)) / best
        if pattern in {"nearly_sorted", "sorted"} and insertion_regret <= 1.05:
            near_sorted_cutoff = max(near_sorted_cutoff, count)
        if medians.get("merge", 999999999) <= best * 1.05:
            merge_thresholds[size_class] = max(merge_thresholds[size_class], count)
        if medians.get("shell", 999999999) <= best * 1.05:
            shell_limits[size_class] = max(shell_limits[size_class], count)
        if medians.get("cycle", 999999999) <= best * 1.05:
            cycle_min_count = max(cycle_min_count, count)

    profile = _generic_profile()
    max_count_seen = max(scenario[1] for scenario in scenario_medians)
    profile["small_count_cutoff"] = max_count_seen
    profile["stable_insertion_cutoff"] = max_count_seen
    profile["near_sorted_insertion_max_count"] = max_count_seen
    profile["shell_max_count_elem_small"] = 0
    profile["shell_max_count_elem_medium"] = 0
    profile["shell_max_count_elem_large"] = 0
    profile["merge_min_count_elem_small"] = 0
    profile["merge_min_count_elem_medium"] = 0
    profile["merge_min_count_elem_large"] = 0
    profile["cycle_min_count"] = 0

    # Second pass: evaluate the selected algorithm under the derived profile.
    for scenario, medians in scenario_medians.items():
        best = max(1, min(medians.values()))
        selected = _select_algorithm(profile, scenario, medians)
        selected_regrets[scenario] = max(1, medians[selected]) / best

    all_regrets = list(selected_regrets.values())
    max_regret = max(all_regrets)
    p95_regret = statistics.quantiles(all_regrets, n=20)[18] if len(all_regrets) >= 20 else max_regret
    if p95_regret > 1.20 or max_regret > 2.00:
        raise ValueError(f"regret gates failed: p95={p95_regret:.3f} max={max_regret:.3f}")

    _write_header(profile, output_h, digest)
    _write_report(rows, profile, output_md, {scenario: {"selected": regret} for scenario, regret in selected_regrets.items()})
    print(f"rows={len(rows)} p95_regret={p95_regret:.3f} max_regret={max_regret:.3f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
