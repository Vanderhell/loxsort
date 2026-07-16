from __future__ import annotations

import csv
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PYTHON = sys.executable
VALIDATE = ROOT / "tools" / "validate_benchmark.py"
REPORT = ROOT / "tools" / "generate_benchmark_report.py"

FIELDNAMES = [
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
]


def _nan() -> str:
    return "nan"


def make_row(
    dataset_id: int,
    family: str,
    data_type: str,
    comparator_kind: str,
    algorithm: str,
    selected_algorithm: str,
    best_algorithm: str,
    selected_sort_ns: str,
    median_ns: str,
    public_api_total_ns: str,
    *,
    scratch_mode: str = "ample_scratch",
    regret: str = "1.000",
    absolute_loss_ns: str = "0.000",
    timing_iterations: str = "3",
    sample_count: str = "3",
    elapsed_ticks_total: str = "300",
    ticks_per_iteration: str = "100",
    feature_extraction_ns: str = _nan(),
    dispatch_only_ns: str = _nan(),
    dispatcher_overhead_ns: str = _nan(),
    dispatcher_overhead_percent: str = _nan(),
) -> dict[str, str]:
    base = {
        "campaign_id": "unit",
        "dataset_id": str(dataset_id),
        "data_family": family,
        "data_type": data_type,
        "pattern": "random",
        "count": "1",
        "element_size": "1",
        "data_bytes": "1",
        "comparator_kind": comparator_kind,
        "seed": str(100 + dataset_id),
        "scratch_mode": scratch_mode,
        "algorithm": algorithm,
        "selected_algorithm": selected_algorithm,
        "best_algorithm": best_algorithm,
        "clock_frequency_hz": "10000000",
        "timing_iterations": timing_iterations,
        "sample_count": sample_count,
        "elapsed_ticks_total": elapsed_ticks_total,
        "ticks_per_iteration": ticks_per_iteration,
        "min_ns": median_ns,
        "median_ns": median_ns,
        "mean_ns": median_ns,
        "p95_ns": median_ns,
        "max_ns": median_ns,
        "feature_extraction_ns": feature_extraction_ns,
        "dispatch_only_ns": dispatch_only_ns,
        "selected_sort_ns": selected_sort_ns,
        "public_api_total_ns": public_api_total_ns,
        "dispatcher_overhead_ns": dispatcher_overhead_ns,
        "dispatcher_overhead_percent": dispatcher_overhead_percent,
        "comparison_count": "1",
        "move_count": "0",
        "swap_count": "0",
        "bytes_moved": "0",
        "sorted_ok": "1",
        "multiset_ok": "1",
        "payload_ok": "1",
        "canary_ok": "1",
        "decision_reason": "11" if selected_algorithm != "none" else "0",
        "profile_fallback_used": "0",
        "sampled_pair_count": "1",
        "disorder_score": "1",
        "equal_score": "0",
        "equal_pair_count": "0",
        "direction_changes": "0",
        "comparison_sign_mask": "0",
        "regret": regret,
        "absolute_loss_ns": absolute_loss_ns,
        "sample_preview": "[]",
    }
    return base


FAMILIES = [
    (0, "integers", "int8_t", "numeric"),
    (1, "floating_point", "float", "total_order"),
    (2, "characters", "raw_bytes", "unsigned"),
    (3, "strings", "char[8]", "case_sensitive"),
    (4, "records", "sensor_record", "timestamp"),
    (5, "timestamps", "uint32_t ticks", "timestamp"),
]


def build_csv(*, bad_oracle: bool = False, missing_merge_dataset: bool = False, timing_scope_mismatch: bool = False, dispatcher_fastest: bool = False) -> str:
    rows: list[dict[str, str]] = []
    for dataset_id, family, data_type, comparator_kind in FAMILIES:
        shell = "100"
        intro = "80"
        merge = "60"
        dispatcher_selected = "80"
        dispatcher_public = "100"
        if dispatcher_fastest:
            dispatcher_selected = "60"
            dispatcher_public = "90"
        oracle_time = merge
        if bad_oracle and dataset_id == 0:
            oracle_time = "90"
        merge_selected = "none" if missing_merge_dataset and dataset_id == 1 else "merge"
        merge_selected_sort = _nan() if merge_selected == "none" else merge
        merge_regret = "1.000" if merge_selected == "merge" else "0.000"
        merge_loss = "0.000" if merge_selected == "merge" else "0.000"
        fixed_public_api = _nan()
        if timing_scope_mismatch and dataset_id == 2:
            fixed_public_api = "55"

        rows.extend(
            [
                make_row(dataset_id, family, data_type, comparator_kind, "always_insertion", "insertion", "merge", "150", "150", _nan()),
                make_row(dataset_id, family, data_type, comparator_kind, "always_shell", "shell", "merge", shell, shell, _nan()),
                make_row(dataset_id, family, data_type, comparator_kind, "always_intro", "intro", "merge", intro, intro, _nan()),
                make_row(dataset_id, family, data_type, comparator_kind, "always_merge_if_available", merge_selected, "merge", merge_selected_sort, merge_selected_sort, _nan(), regret=merge_regret, absolute_loss_ns=merge_loss),
                make_row(dataset_id, family, data_type, comparator_kind, "always_cycle_if_available", "none", "merge", _nan(), _nan(), _nan(), timing_iterations="0", sample_count="0", elapsed_ticks_total="0", ticks_per_iteration="0", regret="1.000", absolute_loss_ns="0.000"),
                make_row(dataset_id, family, data_type, comparator_kind, "loxsort_dispatcher", "intro" if not dispatcher_fastest else "merge", "merge", dispatcher_selected, dispatcher_public, dispatcher_public, feature_extraction_ns="20", dispatch_only_ns="10", dispatcher_overhead_ns="20" if not dispatcher_fastest else "30", dispatcher_overhead_percent="25.000" if not dispatcher_fastest else "50.000", regret="1.000", absolute_loss_ns="0.000"),
                make_row(dataset_id, family, data_type, comparator_kind, "oracle_best", "merge", "merge", oracle_time, oracle_time, _nan(), regret="1.000", absolute_loss_ns="0.000"),
            ]
        )

        if timing_scope_mismatch and dataset_id == 2:
            for row in rows[-7:]:
                if row["algorithm"] == "always_shell":
                    row["public_api_total_ns"] = fixed_public_api

    return _write_csv(rows)


def _write_csv(rows: list[dict[str, str]]) -> str:
    tmp = tempfile.NamedTemporaryFile("w", newline="", encoding="utf-8", delete=False)
    with tmp:
        writer = csv.DictWriter(tmp, fieldnames=FIELDNAMES)
        writer.writeheader()
        writer.writerows(rows)
    return tmp.name


def run_script(script: Path, *args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run([PYTHON, str(script), *args], capture_output=True, text=True)


class BenchmarkToolTests(unittest.TestCase):
    def test_valid_oracle_case(self) -> None:
        csv_path = build_csv()
        with tempfile.TemporaryDirectory() as tmpdir:
            md_path = Path(tmpdir) / "report.md"
            validate = run_script(VALIDATE, csv_path)
            self.assertEqual(validate.returncode, 0, validate.stderr)
            report = run_script(REPORT, csv_path, str(md_path))
            self.assertEqual(report.returncode, 0, report.stderr)
            report_text = md_path.read_text(encoding="utf-8")
            self.assertIn("selected_algorithm_total: 480.000 ns", report_text)
            self.assertIn("public_api_total: 600.000 ns", report_text)
            self.assertIn("| oracle_best | 360.000 ns", report_text)
            self.assertIn("| loxsort_dispatcher | 480.000 ns", report_text)

    def test_dispatcher_selects_fastest_case(self) -> None:
        csv_path = build_csv(dispatcher_fastest=True)
        with tempfile.TemporaryDirectory() as tmpdir:
            md_path = Path(tmpdir) / "report.md"
            validate = run_script(VALIDATE, csv_path)
            self.assertEqual(validate.returncode, 0, validate.stderr)
            report = run_script(REPORT, csv_path, str(md_path))
            self.assertEqual(report.returncode, 0, report.stderr)
            report_text = md_path.read_text(encoding="utf-8")
            self.assertIn("| oracle_best | 360.000 ns", report_text)
            self.assertIn("| loxsort_dispatcher | 360.000 ns", report_text)

    def test_invalid_oracle_case(self) -> None:
        csv_path = build_csv(bad_oracle=True)
        validate = run_script(VALIDATE, csv_path)
        self.assertNotEqual(validate.returncode, 0)
        self.assertIn("oracle time exceeds", validate.stderr)

    def test_dataset_set_mismatch(self) -> None:
        csv_path = build_csv(missing_merge_dataset=True)
        validate = run_script(VALIDATE, csv_path)
        self.assertNotEqual(validate.returncode, 0)
        self.assertIn("merge-eligible", validate.stderr)

    def test_timing_scope_mismatch(self) -> None:
        csv_path = build_csv(timing_scope_mismatch=True)
        validate = run_script(VALIDATE, csv_path)
        self.assertNotEqual(validate.returncode, 0)
        self.assertIn("incompatible timing scope", validate.stderr)


if __name__ == "__main__":
    unittest.main()
