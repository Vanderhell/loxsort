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


def _load_rows(path: Path):
    with path.open("r", newline="", encoding="utf-8-sig") as fh:
        rows = list(csv.DictReader(fh))
    if not rows:
        raise ValueError("empty benchmark summary")
    missing = REQUIRED_COLUMNS.difference(rows[0].keys())
    if missing:
        raise ValueError(f"missing columns: {sorted(missing)}")
    return rows


def _median(values):
    return int(statistics.median(values))


def _percentile(values, percentile):
    if not values:
        return 0.0
    if len(values) == 1:
        return float(values[0])
    ordered = sorted(values)
    rank = (len(ordered) - 1) * percentile
    lower = int(rank)
    upper = min(lower + 1, len(ordered) - 1)
    fraction = rank - lower
    return ordered[lower] + (ordered[upper] - ordered[lower]) * fraction


def _generic_profile():
    return {
        "format_version": 3,
        "algorithm_mask": 31,
        "small_count_cutoff": 16,
        "duplicate_intro_min_count": 16,
        "duplicate_intro_max_count": 64,
        "stable_insertion_cutoff": 32,
        "near_sorted_insertion_max_count": 64,
        "cycle_min_count": 64,
        "shell_max_count_elem_small": 256,
        "shell_max_count_elem_medium": 512,
        "shell_max_count_elem_large": 1024,
        "merge_min_count_elem_small": 0,
        "merge_min_count_elem_medium": 0,
        "merge_min_count_elem_large": 0,
        "sample_pairs": 16,
        "near_sorted_disorder_max": 32,
        "near_sorted_direction_changes_max": 4,
        "duplicate_equal_min": 6,
        "duplicate_disorder_min": 96,
        "cycle_equal_score_max": 32,
        "reserved0": 0,
    }


def _scenario_key(row):
    return (
        row["split"].strip().lower(),
        row["pattern"],
        int(row["count"]),
        int(row["element_size"]),
        int(row["dataset_seed"]),
        int(row["sampled_pair_count"]),
        int(row["disorder_score"]),
        int(row["equal_score"]),
        int(row["equal_pair_count"]),
        int(row["has_less"]),
        int(row["has_greater"]),
        int(row["direction_changes"]),
    )


def _group_rows(rows):
    by_scenario = defaultdict(lambda: defaultdict(list))
    for row in rows:
        if row["correctness"].strip().lower() != "ok":
            raise ValueError(f"incorrect row for {row['algorithm']} {row['pattern']}")
        by_scenario[_scenario_key(row)][row["algorithm"]].append(int(row["elapsed_ticks"]))
    return by_scenario


def _split_summary(groups):
    seeds = sorted({scenario[3] for scenario in groups})
    scenario_lines = []
    for scenario in sorted(groups):
        scenario_lines.append("|".join(str(part) for part in scenario))
    digest_source = "\n".join(scenario_lines).encode("utf-8")
    return {
        "count": len(groups),
        "seeds": seeds,
        "digest": hashlib.sha256(digest_source).hexdigest(),
    }


def _algorithm_class(element_size):
    if element_size <= 4:
        return "small"
    if element_size <= 16:
        return "medium"
    return "large"


def _select_best_algorithm(medians):
    best_algorithm = None
    best_time = None
    for algorithm, elapsed in medians.items():
        if best_time is None or elapsed < best_time or (elapsed == best_time and algorithm < best_algorithm):
            best_algorithm = algorithm
            best_time = elapsed
    return best_algorithm, best_time


def _derive_profile(training_groups):
    _ = training_groups
    return _generic_profile()


def _profile_complexity(profile):
    shell_nonzero = sum(
        1
        for key in (
            "shell_max_count_elem_small",
            "shell_max_count_elem_medium",
            "shell_max_count_elem_large",
        )
        if profile[key] != 0
    )
    threshold_sum = sum(
        int(profile[key])
        for key in (
            "small_count_cutoff",
            "duplicate_intro_min_count",
            "duplicate_intro_max_count",
            "stable_insertion_cutoff",
            "near_sorted_insertion_max_count",
            "cycle_min_count",
            "shell_max_count_elem_small",
            "shell_max_count_elem_medium",
            "shell_max_count_elem_large",
            "merge_min_count_elem_small",
            "merge_min_count_elem_medium",
            "merge_min_count_elem_large",
            "near_sorted_disorder_max",
            "near_sorted_direction_changes_max",
            "duplicate_equal_min",
            "duplicate_disorder_min",
            "cycle_equal_score_max",
        )
    )
    return shell_nonzero, threshold_sum


def _evaluate_profile(profile, scenarios):
    regrets = []
    for scenario, algorithms in scenarios.items():
        medians = {algorithm: _median(times) for algorithm, times in algorithms.items()}
        _, best_time = _select_best_algorithm(medians)
        selected_algorithm = _dispatch_choice(profile, scenario)
        selected_time = medians.get(selected_algorithm)
        if selected_time is None:
            raise ValueError(f"selected algorithm {selected_algorithm} missing for {scenario}")
        regrets.append(max(1.0, selected_time / max(1, best_time)))
    return regrets


def _regret_map(profile, scenarios):
    regrets = {}
    for scenario, algorithms in scenarios.items():
        medians = {algorithm: _median(times) for algorithm, times in algorithms.items()}
        _, best_time = _select_best_algorithm(medians)
        selected_algorithm = _dispatch_choice(profile, scenario)
        selected_time = medians.get(selected_algorithm)
        if selected_time is None:
            raise ValueError(f"selected algorithm {selected_algorithm} missing for {scenario}")
        regrets[scenario] = max(1.0, selected_time / max(1, best_time))
    return regrets


def _validation_case_records(profile, scenarios):
    records = []
    for scenario, algorithms in scenarios.items():
        medians = {algorithm: _median(times) for algorithm, times in algorithms.items()}
        best_algorithm, best_time = _select_best_algorithm(medians)
        selected_algorithm = _dispatch_choice(profile, scenario)
        selected_time = medians.get(selected_algorithm)
        if selected_time is None:
            raise ValueError(f"selected algorithm {selected_algorithm} missing for {scenario}")
        regret = max(1.0, selected_time / max(1, best_time))
        records.append(
            {
                "scenario": scenario,
                "selected_algorithm": selected_algorithm,
                "best_algorithm": best_algorithm,
                "selected_time": selected_time,
                "best_time": best_time,
                "regret": regret,
            }
        )
    return records


def _profile_score(profile, scenarios):
    regrets = _evaluate_profile(profile, scenarios)
    p50 = _percentile(regrets, 0.50)
    p95 = _percentile(regrets, 0.95)
    maximum = max(regrets) if regrets else 0.0
    generic = _generic_profile()
    distance = (
        abs(int(profile["small_count_cutoff"]) - int(generic["small_count_cutoff"])) +
        abs(int(profile["duplicate_intro_min_count"]) - int(generic["duplicate_intro_min_count"])) +
        abs(int(profile["duplicate_intro_max_count"]) - int(generic["duplicate_intro_max_count"])) +
        abs(int(profile["stable_insertion_cutoff"]) - int(generic["stable_insertion_cutoff"])) +
        abs(int(profile["near_sorted_insertion_max_count"]) - int(generic["near_sorted_insertion_max_count"])) +
        abs(int(profile["near_sorted_disorder_max"]) - int(generic["near_sorted_disorder_max"])) +
        abs(int(profile["near_sorted_direction_changes_max"]) - int(generic["near_sorted_direction_changes_max"])) +
        abs(int(profile["duplicate_equal_min"]) - int(generic["duplicate_equal_min"])) +
        abs(int(profile["duplicate_disorder_min"]) - int(generic["duplicate_disorder_min"])) +
        abs(int(profile["shell_max_count_elem_small"]) - int(generic["shell_max_count_elem_small"])) +
        abs(int(profile["shell_max_count_elem_medium"]) - int(generic["shell_max_count_elem_medium"])) +
        abs(int(profile["shell_max_count_elem_large"]) - int(generic["shell_max_count_elem_large"]))
    )
    return (p95, maximum, p50, distance, *_profile_complexity(profile))


def _candidate_profiles(base_profile):
    candidates = []
    seen = set()

    def add(profile):
        key = tuple(sorted(profile.items()))
        if key not in seen:
            seen.add(key)
            candidates.append(profile)

    add(dict(base_profile))
    add(_generic_profile())

    for small_value in {4, 6, 8, base_profile["small_count_cutoff"]}:
        for duplicate_min in {16, 23, 24, base_profile["duplicate_intro_min_count"]}:
            for duplicate_max in {24, 31, 64, base_profile["duplicate_intro_max_count"]}:
                if duplicate_max < duplicate_min:
                    continue
                for equal_min in {1, 2, 4, 6, base_profile["duplicate_equal_min"]}:
                    for disorder_min in {64, 80, 96, 112, base_profile["duplicate_disorder_min"]}:
                        for disorder_value in {20, 24, 28, 32, base_profile["near_sorted_disorder_max"]}:
                            for direction_value in {2, 4, 6, base_profile["near_sorted_direction_changes_max"]}:
                                trial = dict(base_profile)
                                trial["small_count_cutoff"] = small_value
                                trial["duplicate_intro_min_count"] = duplicate_min
                                trial["duplicate_intro_max_count"] = duplicate_max
                                trial["duplicate_equal_min"] = equal_min
                                trial["duplicate_disorder_min"] = disorder_min
                                trial["near_sorted_disorder_max"] = disorder_value
                                trial["near_sorted_direction_changes_max"] = direction_value
                                trial["stable_insertion_cutoff"] = max(trial["small_count_cutoff"], 32)
                                add(trial)

    return candidates


def _search_training_profile(training_groups):
    profile = _derive_profile(training_groups)
    count_candidates = sorted(
        {4, 8, 16, 23, 24, 31, 32, 47, 48, 63, 64}
        | {scenario[1] for scenario in training_groups if scenario[1] <= 64}
    )
    disorder_candidates = [8, 12, 16, 20, 24, 28, 32]
    direction_candidates = [0, 1, 2, 3, 4, 5, 6, 8, 10, 12, 16]
    duplicate_equal_candidates = [1, 2, 3, 4, 5, 6, 8, 10, 12, 14, 16]
    duplicate_disorder_candidates = [0, 64, 80, 96, 112, 128, 144, 160, 192, 224, 255]
    best_score = _profile_score(profile, training_groups)

    def try_update(trial_profile):
        nonlocal profile, best_score
        score = _profile_score(trial_profile, training_groups)
        if score < best_score:
            profile = trial_profile
            best_score = score
            return True
        return False

    improved = True
    while improved:
        improved = False

        for small_value in count_candidates:
            trial = dict(profile)
            trial["small_count_cutoff"] = small_value
            if try_update(trial):
                improved = True

        for duplicate_min in count_candidates:
            for duplicate_max in count_candidates:
                if duplicate_max < duplicate_min:
                    continue
                trial = dict(profile)
                trial["duplicate_intro_min_count"] = duplicate_min
                trial["duplicate_intro_max_count"] = duplicate_max
                if try_update(trial):
                    improved = True

        for equal_min in duplicate_equal_candidates:
            trial = dict(profile)
            trial["duplicate_equal_min"] = equal_min
            if try_update(trial):
                improved = True

        for disorder_min in duplicate_disorder_candidates:
            trial = dict(profile)
            trial["duplicate_disorder_min"] = disorder_min
            if try_update(trial):
                improved = True

        for disorder_value in disorder_candidates:
            trial = dict(profile)
            trial["near_sorted_disorder_max"] = disorder_value
            if try_update(trial):
                improved = True

        for direction_value in direction_candidates:
            trial = dict(profile)
            trial["near_sorted_direction_changes_max"] = direction_value
            if try_update(trial):
                improved = True

    profile["stable_insertion_cutoff"] = max(profile["small_count_cutoff"], 32)
    return profile, _profile_score(profile, training_groups)


def _select_profile_by_validation(training_profile, validation_groups):
    best_profile = None
    best_score = None
    for candidate in _candidate_profiles(training_profile):
        score = _profile_score(candidate, validation_groups)
        if best_score is None or score < best_score:
            best_profile = candidate
            best_score = score
    if best_profile is None:
        raise ValueError("no validation candidates available")
    return best_profile, best_score


def _dispatch_choice(profile, scenario):
    _, count, element_size, _, sampled_pair_count, disorder_score, equal_score, equal_pair_count, has_less, has_greater, direction_changes = scenario
    size_class = _algorithm_class(element_size)
    if count <= profile["small_count_cutoff"]:
        return "insertion"
    if (
        profile["duplicate_intro_min_count"] <= count <= profile["duplicate_intro_max_count"]
        and equal_pair_count >= profile["duplicate_equal_min"]
        and has_less
        and has_greater
        and disorder_score >= profile["duplicate_disorder_min"]
    ):
        return "intro"
    if disorder_score <= profile["near_sorted_disorder_max"]:
        if count <= profile["near_sorted_insertion_max_count"] and direction_changes <= profile["near_sorted_direction_changes_max"]:
            return "insertion"
        if direction_changes > profile["near_sorted_direction_changes_max"]:
            return "intro"
    if count <= profile[f"shell_max_count_elem_{size_class}"] and profile[f"shell_max_count_elem_{size_class}"] != 0:
        return "shell"
    if profile["cycle_min_count"] != 0 and count >= profile["cycle_min_count"] and equal_score <= profile["cycle_equal_score_max"]:
        return "cycle"
    return "intro"


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
        f"    {profile['duplicate_intro_min_count']}u,",
        f"    {profile['duplicate_intro_max_count']}u,",
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
        f"    {profile['near_sorted_direction_changes_max']}u,",
        f"    {profile['duplicate_equal_min']}u,",
        f"    {profile['duplicate_disorder_min']}u,",
        f"    {profile['cycle_equal_score_max']}u,",
        f"    {profile['reserved0']}u",
        "};",
        "",
        "#endif",
        "",
    ]
    out_path.write_text("\n".join(lines), encoding="utf-8")


def _compare_regret_maps(selected, baseline):
    improved = worsened = same = 0
    for scenario, selected_regret in selected.items():
        baseline_regret = baseline[scenario]
        if selected_regret < baseline_regret:
            improved += 1
        elif selected_regret > baseline_regret:
            worsened += 1
        else:
            same += 1
    return improved, worsened, same


def _write_report(
    profile,
    out_path: Path,
    training_groups,
    validation_groups,
    test_groups,
    training_regrets,
    baseline_training_regrets,
    validation_regrets,
    baseline_validation_regrets,
    test_regrets,
    baseline_test_regrets,
):
    training_improved, training_worsened, training_same = _compare_regret_maps(training_regrets, baseline_training_regrets)
    validation_improved, validation_worsened, validation_same = _compare_regret_maps(validation_regrets, baseline_validation_regrets)
    test_improved, test_worsened, test_same = _compare_regret_maps(test_regrets, baseline_test_regrets)
    training_summary = _split_summary(training_groups)
    validation_summary = _split_summary(validation_groups)
    test_summary = _split_summary(test_groups)
    validation_records = _validation_case_records(profile, validation_groups)
    lines = [
        "# LoxSort profile report",
        "",
        "## Selected profile",
    ]
    for key in [
        "format_version",
        "algorithm_mask",
        "small_count_cutoff",
        "duplicate_intro_min_count",
        "duplicate_intro_max_count",
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
        "near_sorted_direction_changes_max",
        "duplicate_equal_min",
        "duplicate_disorder_min",
        "cycle_equal_score_max",
    ]:
        lines.append(f"- {key}: {profile[key]}")

    lines.extend([
        "",
        "## Dataset split",
        f"- training scenarios: {len(training_groups)}",
        f"- validation scenarios: {len(validation_groups)}",
        f"- test scenarios: {len(test_groups)}",
        f"- training seeds: {training_summary['seeds']}",
        f"- validation seeds: {validation_summary['seeds']}",
        f"- test seeds: {test_summary['seeds']}",
        f"- training digest: {training_summary['digest']}",
        f"- validation digest: {validation_summary['digest']}",
        f"- test digest: {test_summary['digest']}",
        "",
        "## Regression guard",
        f"- training improved: {training_improved}",
        f"- training worsened: {training_worsened}",
        f"- training unchanged: {training_same}",
        f"- validation improved: {validation_improved}",
        f"- validation worsened: {validation_worsened}",
        f"- validation unchanged: {validation_same}",
        f"- test improved: {test_improved}",
        f"- test worsened: {test_worsened}",
        f"- test unchanged: {test_same}",
        "",
        "## Split regret",
    ])
    for split_name, regrets_map in (
        ("training", training_regrets),
        ("validation", validation_regrets),
        ("test", test_regrets),
    ):
        if regrets_map:
            regrets = list(regrets_map.values())
            worst = sorted(regrets_map.items(), key=lambda item: item[1], reverse=True)[:5]
            lines.append(f"### {split_name}")
            lines.append(f"- reported scenarios: {len(regrets_map)}")
            lines.append("- worst cases:")
            for scenario, regret in worst:
                lines.append(f"  - {scenario}: regret={regret:.3f}")
            lines.extend([
                "",
                f"- p50_regret: {_percentile(regrets, 0.50):.3f}",
                f"- p95_regret: {_percentile(regrets, 0.95):.3f}",
                f"- max_regret: {max(regrets):.3f}",
                "",
            ])
    if validation_records:
        worst_validation = sorted(validation_records, key=lambda item: item["regret"], reverse=True)[:10]
        lines.append("## Worst validation cases")
        for record in worst_validation:
            scenario = record["scenario"]
            pattern = scenario[0]
            count = scenario[1]
            element_size = scenario[2]
            features = {
                "dataset_seed": scenario[3],
                "sampled_pair_count": scenario[4],
                "disorder_score": scenario[5],
                "equal_score": scenario[6],
                "equal_pair_count": scenario[7],
                "has_less": scenario[8],
                "has_greater": scenario[9],
                "direction_changes": scenario[10],
            }
            feature_text = ", ".join(f"{key}={value}" for key, value in features.items())
            lines.extend([
                f"- pattern: {pattern}",
                f"  count: {count}",
                f"  element_size: {element_size}",
                f"  features: {feature_text}",
                f"  selected_algorithm: {record['selected_algorithm']}",
                f"  best_algorithm: {record['best_algorithm']}",
                f"  regret: {record['regret']:.3f}",
            ])
    out_path.write_text("\n".join(lines).rstrip() + "\n", encoding="utf-8")


def main(argv: list[str]) -> int:
    if len(argv) != 4:
        print("usage: generate_profile.py input.csv output.h output.md", file=sys.stderr)
        return 2

    input_csv = Path(argv[1])
    output_h = Path(argv[2])
    output_md = Path(argv[3])
    rows = _load_rows(input_csv)
    digest = hashlib.sha256(input_csv.read_bytes()).hexdigest()

    grouped_rows = _group_rows(rows)
    training_groups = {}
    validation_groups = {}
    test_groups = {}

    for scenario, algorithms in grouped_rows.items():
        split = scenario[0]
        scenario_key = scenario[1:]
        if split == "train":
            training_groups[scenario_key] = algorithms
        elif split == "validation":
            validation_groups[scenario_key] = algorithms
        elif split == "test":
            test_groups[scenario_key] = algorithms
        else:
            raise ValueError(f"unexpected split: {split}")

    if not training_groups:
        raise ValueError("no training rows found")
    if not validation_groups:
        raise ValueError("no validation rows found")
    if not test_groups:
        raise ValueError("no test rows found")

    training_profile, training_score = _search_training_profile(training_groups)
    profile, validation_score = _select_profile_by_validation(training_profile, validation_groups)
    generic = _generic_profile()

    training_regrets = _regret_map(profile, training_groups)
    baseline_training_regrets = _regret_map(generic, training_groups)
    validation_regrets = _regret_map(profile, validation_groups)
    baseline_validation_regrets = _regret_map(generic, validation_groups)
    test_regrets = _regret_map(profile, test_groups)
    baseline_test_regrets = _regret_map(generic, test_groups)

    validation_values = list(validation_regrets.values())
    test_values = list(test_regrets.values())
    validation_p95 = _percentile(validation_values, 0.95)
    validation_max = max(validation_values)
    test_p95 = _percentile(test_values, 0.95)
    test_max = max(test_values)
    if test_p95 > 3.00 or test_max > 6.00:
        raise ValueError(f"test regret gates failed: p95={test_p95:.3f} max={test_max:.3f}")

    _write_header(profile, output_h, digest)
    _write_report(
        profile,
        output_md,
        training_groups,
        validation_groups,
        test_groups,
        training_regrets,
        baseline_training_regrets,
        validation_regrets,
        baseline_validation_regrets,
        test_regrets,
        baseline_test_regrets,
    )
    training_improved, training_worsened, _ = _compare_regret_maps(training_regrets, baseline_training_regrets)
    validation_improved, validation_worsened, _ = _compare_regret_maps(validation_regrets, baseline_validation_regrets)
    test_improved, test_worsened, _ = _compare_regret_maps(test_regrets, baseline_test_regrets)
    print(
        f"rows={len(rows)} training_groups={len(training_groups)} validation_groups={len(validation_groups)} test_groups={len(test_groups)} "
        f"train_p95_regret={training_score[0]:.3f} train_max_regret={training_score[1]:.3f} "
        f"validation_p95_regret={validation_score[0]:.3f} validation_max_regret={validation_score[1]:.3f} "
        f"train_improved={training_improved} train_worsened={training_worsened} "
        f"validation_improved={validation_improved} validation_worsened={validation_worsened} "
        f"test_improved={test_improved} test_worsened={test_worsened} "
        f"validation_p95={validation_p95:.3f} validation_max={validation_max:.3f} "
        f"test_p95={test_p95:.3f} test_max={test_max:.3f}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
