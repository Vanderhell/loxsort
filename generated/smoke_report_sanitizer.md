# Benchmark Report

## Metadata
- git_commit: 72229e5863f51ff5952258cee1632e563d906031
- git_state: dirty
- os: Windows-10-10.0.26100-SP0
- cpu: Intel64 Family 6 Model 79 Stepping 1, GenuineIntel
- compiler: ClangCL
- compiler_version: NOT AVAILABLE
- build_configuration: Debug
- optimization_flags: Sanitizers
- cmake_options: -DLOXSORT_ENABLE_SANITIZERS=ON -DLOXSORT_BUILD_BENCH=ON -DLOXSORT_BUILD_TESTS=ON
- clock_source: QueryPerformanceCounter
- clock_frequency_hz: 10000000
- campaign_command: build_san_bench\Debug\loxsort_bench.exe --campaign smoke --seed-base 1 --output generated\smoke_bench_sanitizer.csv --checkpoint-interval 10 --timing-target-ms 2 --timing-samples 3
- run_timestamp: 2026-06-14T19:02:41.673436+02:00
- csv_path: generated\smoke_bench_sanitizer.csv
- scenario_digest: db2b22da3da511b5c03f3b6fab34761ba06f64aca011126c7365b8c24565ee65
- planned_dataset_count: 100
- executed_dataset_count: 100
- row_count: 700

## Overall Strategy Results

| strategy | total_time | mean_time | median_time | geometric_mean_speedup | p95_time | p99_time | max_time | wins | losses | ties |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| always_insertion | 224.106 ms | 2.241 ms | 27.137 us | 0.710x | 5.472 ms | 17.457 ms | 154.361 ms | 27 | 73 | 27 |
| always_shell | 17.692 ms | 176.917 us | 18.806 us | 1.202x | 671.075 us | 3.521 ms | 3.941 ms | 25 | 75 | 21 |
| always_intro | 46.827 ms | 468.271 us | 23.256 us | 1.097x | 1.250 ms | 4.872 ms | 24.055 ms | 21 | 79 | 19 |
| always_merge_if_available | 8.478 ms | 84.783 us | 478.723 ns | 1.245x | 290.700 us | 2.336 ms | 3.058 ms | 27 | 73 | 73 |
| always_cycle_if_available | 0.000 ns | 0.000 ns | 0.000 ns | NA | 0.000 ns | 0.000 ns | 0.000 ns | 0 | 100 | 100 |
| loxsort_dispatcher | 7.709 ms | 82.889 us | 26.443 us | 1.000x | 255.312 us | 975.567 us | 1.195 ms | 36 | 64 | 13 |
| oracle_best | 10.791 ms | 107.914 us | 12.968 us | 1.557x | 647.475 us | 1.195 ms | 2.336 ms | 0 | 0 | 0 |

## LoxSort Quality

- p50_regret: 1.519
- p95_regret: 4.735
- p99_regret: 5.992
- max_regret: 7.530
- mean_absolute_loss: 16.812 us
- median_absolute_loss: 3.684 us
- total_absolute_loss: 1.681 ms
- dispatcher_overhead_median: 10.640 ns
- dispatcher_overhead_p95: NA

## Merge-Eligible Results

- eligible_dataset_count: 54

| strategy | total_time | mean_time | median_time | p95_time | wins | losses | ties | speedup_vs_dispatcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| loxsort_dispatcher | 2.658 ms | 52.108 us | 21.353 us | 182.936 us | 14 | 40 | 5 | 1.000x |
| always_shell | 11.159 ms | 206.640 us | 15.977 us | 458.260 us | 4 | 50 | 4 | 0.238x |
| always_intro | 37.656 ms | 697.325 us | 22.406 us | 1.326 ms | 9 | 45 | 7 | 0.071x |
| always_merge_if_available | 8.478 ms | 157.005 us | 16.586 us | 489.440 us | 27 | 27 | 27 | 0.313x |
| oracle_best | 5.789 ms | 107.207 us | 11.760 us | 290.700 us | 0 | 0 | 0 | 0.459x |

## Coverage

| family | data types | comparators | dataset count | count range | element-size range | patterns used | sample values |
| --- | --- | --- | ---: | --- | --- | --- | --- |
| integers | int16_t, int32_t, int64_t, int8_t, uint16_t, uint32_t, uint64_t, uint8_t | numeric, unsigned | 18 | 0-512 | 1-8 | adversarial, all_equal, ascending_runs, bit_reversal, descending_blocks_4, descending_runs, few_unique_8, nearly_sorted, organ_pipe, plateau, push_front, random, random_half, random_tail, reverse, reverse_first_half, sawtooth | `[]` ; `[12, 12, 12]` ; `[0, 0, 0]` |
| floating_point | double, float | total_order | 8 | 21-256 | 4-8 | all_equal, alternating_high_low, few_unique_8, push_front, random_half, sawtooth, sorted | `[1.84256e-17, -3.14795e-32, -1.8621e-13]` ; `[3.77889595021e+195, 9.4848212551e-238, -3.22461389179e-229]` ; `[2.93106e-20, 2.93106e-20, 2.93106e-20]` |
| characters | alnum, ascii, case_insensitive_ascii, digits, lowercase, raw_bytes, uppercase | case_insensitive, case_sensitive, unsigned | 7 | 4-129 | 1-1 | ascending_runs, descending_blocks_4, plateau, reverse | `["\xC1", "\x93", "e"]` ; `["", "", ""]` ; `["4", "3", "2"]` |
| strings | char[128], char[16], char[32], char[64], char[8], const char * | case_insensitive, case_sensitive | 25 | 2-513 | 8-128 | adversarial, all_equal, bit_reversal, descending_runs, few_unique_2, nearly_sorted, organ_pipe, plateau, push_front, push_middle, random, random_half, random_tail, reverse_first_half, sawtooth, sorted, stagger | `["timesta", "alpha-6", "log-497"]` ; `["log-11983550542", "", ""]` ; `["dev-e40d7bb0", "uuid-00000000-0002", "uuid-00000000-0003"]` |
| records | database_record, device_event, log_record, packet_record, scheduler_task, sensor_record | id, priority, priority_timestamp_id, text_field, timestamp | 30 | 0-4096 | 8-64 | adversarial, all_equal, alternating_high_low, ascending_runs, descending_blocks_4, descending_runs, few_unique_2, few_unique_8, nearly_sorted, organ_pipe, plateau, push_front, push_middle, random, random_half, random_tail, reverse, reverse_first_half, reverse_second_half, sorted | `[{key=0, idx=0}]` ; `[{key=0, idx=0}, {key=0, idx=0}]` ; `[{key=1, idx=15605528932144774638}, {key=2, idx=2816022119691119401}, {key=3, idx=2854415327811902345}]` |
| timestamps | timestamp+device_id, timestamp+priority, timestamp+sequence, uint32_t ticks, uint64_t timestamps | device, priority, priority_timestamp_id, sequence, timestamp | 12 | 0-127 | 4-16 | adversarial, ascending_runs, few_unique_2, few_unique_8, nearly_sorted, plateau, random, random_half, reverse_first_half, reverse_second_half, stagger | `[5, 6, 4]` ; `[4, 0, 0]` ; `[6677463002066809815, 10810347695623708071, 4936069724084857339]` |

## Regret Buckets

### Oracle Absolute Time

| bucket | dataset count | p50 regret | p95 regret | p99 regret | max regret | median absolute loss | total absolute loss |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| < 100 ns | 0 | NA | NA | NA | NA | NA | NA |
| 100 ns to 1 us | 12 | 2.323 | 3.188 | 3.188 | 3.507 | 416.217 ns | 5.369 us |
| 1 us to 10 us | 29 | 1.634 | 3.664 | 4.735 | 5.992 | 2.414 us | 121.404 us |
| 10 us to 100 us | 44 | 1.377 | 5.227 | 5.667 | 7.530 | 15.671 us | 1.193 ms |
| 100 us to 1 ms | 13 | 1.000 | 1.616 | 1.616 | 1.619 | 0.000 ns | 348.905 us |
| >= 1 ms | 2 | 1.005 | 1.000 | 1.000 | 1.010 | 6.175 us | 12.350 us |

### Count

| bucket | dataset count | p50 regret | p95 regret | p99 regret | max regret | median absolute loss | total absolute loss |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 0-3 | 8 | 1.756 | 3.188 | 3.188 | 3.507 | 189.936 ns | 1.684 us |
| 4-8 | 9 | 1.980 | 2.481 | 2.481 | 3.110 | 406.764 ns | 4.796 us |
| 9-32 | 33 | 1.804 | 3.664 | 4.735 | 5.992 | 4.064 us | 222.376 us |
| 33-128 | 30 | 1.289 | 1.725 | 4.507 | 7.530 | 9.324 us | 430.185 us |
| 129-512 | 15 | 1.616 | 5.407 | 5.407 | 5.667 | 52.642 us | 1.022 ms |
| 513+ | 5 | 1.000 | 1.000 | 1.000 | 1.000 | 0.000 ns | 0.000 ns |

## Worst Cases

### Top 100 by regret

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 78.519 us | 10.428 us | 7.530 | 68.092 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 30.031 us | 5.012 us | 5.992 | 25.019 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 63.922 us | 11.280 us | 5.667 | 52.642 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 84.704 us | 15.666 us | 5.407 | 69.038 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 59.297 us | 11.344 us | 5.227 | 47.952 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 15.170 us | 3.204 us | 4.735 | 11.967 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 67.970 us | 15.081 us | 4.507 | 52.890 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 14.179 us | 3.870 us | 3.664 | 10.309 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | intro | 255.312 us | 70.567 us | 3.618 | 184.744 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | insertion | 554.939 ns | 158.238 ns | 3.507 | 396.699 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 553.514 ns | 173.624 ns | 3.188 | 379.873 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | intro | 627.415 ns | 201.741 ns | 3.110 | 425.670 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | insertion | 663.823 ns | 221.940 ns | 2.991 | 441.886 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 234.356 us | 79.147 us | 2.961 | 155.221 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 29.184 us | 10.821 us | 2.697 | 18.364 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 34.192 us | 12.701 us | 2.692 | 21.492 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=0, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 26.416 us | 10.207 us | 2.588 | 16.210 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 148.414 us | 57.794 us | 2.568 | 90.623 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | insertion | 773.627 ns | 307.973 ns | 2.512 | 465.635 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | intro | 681.465 ns | 274.674 ns | 2.481 | 406.764 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 16.963 us | 6.851 us | 2.476 | 10.113 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 2.601 us | 1.055 us | 2.466 | 1.546 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 27.822 us | 11.412 us | 2.438 | 16.411 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | insertion | 740.030 ns | 341.815 ns | 2.165 | 398.266 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 21.353 us | 10.222 us | 2.089 | 11.130 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | intro | 720.346 ns | 350.193 ns | 2.057 | 370.092 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 17.106 us | 8.536 us | 2.004 | 8.571 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 1.525 us | 770.125 ns | 1.980 | 754.832 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 20.961 us | 10.619 us | 1.974 | 10.345 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=0, reason=9 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 8.303 us | 4.239 us | 1.959 | 4.065 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 2.539 us | 1.301 us | 1.951 | 1.237 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 3.958 us | 2.079 us | 1.904 | 1.879 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 957.492 ns | 506.342 ns | 1.891 | 451.137 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | shell | 15.165 us | 8.075 us | 1.878 | 7.092 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 10.367 us | 5.747 us | 1.804 | 4.619 us | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 15.006 us | 8.512 us | 1.763 | 6.496 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 84.800 us | 49.159 us | 1.725 | 35.637 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 4.444 us | 2.633 us | 1.688 | 1.812 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 26.443 us | 15.778 us | 1.676 | 10.665 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 84.542 us | 51.300 us | 1.648 | 33.257 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 65.981 us | 40.232 us | 1.640 | 25.749 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | shell | 3.386 us | 2.071 us | 1.635 | 1.315 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 7.066 us | 4.324 us | 1.634 | 2.741 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 203.540 us | 125.720 us | 1.619 | 77.815 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 469.720 us | 290.668 us | 1.616 | 179.020 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | intro | 1.050 us | 662.746 ns | 1.584 | 387.033 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 4.080 us | 2.607 us | 1.565 | 1.473 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 139.420 us | 89.487 us | 1.558 | 49.937 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 58.729 us | 37.816 us | 1.553 | 20.923 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 69.793 us | 45.587 us | 1.531 | 24.218 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 1.459 us | 967.835 ns | 1.507 | 490.797 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | merge | 7.184 us | 4.770 us | 1.506 | 2.414 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | merge | 12.787 us | 8.722 us | 1.466 | 4.064 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=0, reason=9 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 8.040 us | 5.572 us | 1.443 | 2.470 us | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | intro | 8.761 us | 6.191 us | 1.415 | 2.569 us | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 61.839 us | 44.682 us | 1.384 | 17.171 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 83.537 us | 60.976 us | 1.370 | 22.571 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | insertion | 1.879 us | 1.372 us | 1.370 | 507.088 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 49.732 us | 36.541 us | 1.361 | 13.194 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 7.439 us | 5.470 us | 1.360 | 1.968 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 4.016 us | 2.957 us | 1.358 | 1.059 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 59.968 us | 44.819 us | 1.338 | 15.132 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 8.544 us | 6.414 us | 1.332 | 2.132 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 42.564 us | 32.148 us | 1.324 | 10.407 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 45.657 us | 34.536 us | 1.322 | 11.129 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | insertion | 1.656 us | 1.254 us | 1.321 | 402.039 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 11.174 us | 8.662 us | 1.290 | 2.511 us | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 1.712 us | 1.344 us | 1.274 | 367.645 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 104.160 us | 82.930 us | 1.256 | 21.244 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 42.823 us | 34.591 us | 1.238 | 8.241 us | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | merge | 57.000 us | 46.229 us | 1.233 | 10.770 us | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 103.320 us | 85.530 us | 1.208 | 17.770 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | intro | 1.773 us | 1.485 us | 1.194 | 287.807 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 15.771 us | 13.242 us | 1.191 | 2.533 us | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 119.941 us | 107.571 us | 1.115 | 12.383 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 4.150 us | 3.752 us | 1.106 | 397.920 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 39.543 us | 36.245 us | 1.091 | 3.304 us | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | merge | 17.643 us | 16.201 us | 1.089 | 1.443 us | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | shell | 975.567 us | 904.978 us | 1.078 | 70.967 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=0, reason=8 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 73.729 us | 68.394 us | 1.078 | 5.359 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 75.107 us | 69.867 us | 1.075 | 5.238 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 93.032 us | 86.622 us | 1.074 | 6.436 us | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 20.608 us | 19.423 us | 1.061 | 1.177 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=0, reason=9 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 53.953 us | 50.995 us | 1.058 | 2.963 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 28.751 us | 27.175 us | 1.058 | 1.577 us | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 500.780 us | 491.925 us | 1.018 | 8.720 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | shell | 1.195 ms | 1.183 ms | 1.010 | 12.350 us | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 0 | integers | int8_t | random | 0 | 1 | 1 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 229.456 us | 229.456 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=11 |
| 4 | records | sensor_record | few_unique_2 | 1 | 8 | 12780580985233474033 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 182.936 us | 182.936 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 43 | records | database_record | plateau | 2048 | 48 | 2727476683324678604 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 63 | records | log_record | reverse_first_half | 4096 | 24 | 882992644293677392 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 55.986 us | 55.986 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | shell | 647.475 us | 647.475 us | 1.000 | 0.000 ns | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=8 |
| 76 | records | sensor_record | all_equal | 0 | 8 | 9829787259463779993 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 83 | records | sensor_record | random_tail | 1237 | 8 | 17485252678972227796 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 86 | timestamps | timestamp+sequence | few_unique_2 | 0 | 16 | 1126868243982466986 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 46.207 us | 46.207 us | 1.000 | 0.000 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 71.241 us | 71.241 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |

### Top 100 by absolute_loss_ns

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | intro | 255.312 us | 70.567 us | 3.618 | 184.744 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 469.720 us | 290.668 us | 1.616 | 179.020 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 234.356 us | 79.147 us | 2.961 | 155.221 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 148.414 us | 57.794 us | 2.568 | 90.623 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 203.540 us | 125.720 us | 1.619 | 77.815 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | shell | 975.567 us | 904.978 us | 1.078 | 70.967 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=0, reason=8 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 84.704 us | 15.666 us | 5.407 | 69.038 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 78.519 us | 10.428 us | 7.530 | 68.092 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 67.970 us | 15.081 us | 4.507 | 52.890 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 63.922 us | 11.280 us | 5.667 | 52.642 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 139.420 us | 89.487 us | 1.558 | 49.937 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 59.297 us | 11.344 us | 5.227 | 47.952 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 84.800 us | 49.159 us | 1.725 | 35.637 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 84.542 us | 51.300 us | 1.648 | 33.257 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 65.981 us | 40.232 us | 1.640 | 25.749 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 30.031 us | 5.012 us | 5.992 | 25.019 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 69.793 us | 45.587 us | 1.531 | 24.218 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 83.537 us | 60.976 us | 1.370 | 22.571 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 34.192 us | 12.701 us | 2.692 | 21.492 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=0, reason=9 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 104.160 us | 82.930 us | 1.256 | 21.244 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 58.729 us | 37.816 us | 1.553 | 20.923 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 29.184 us | 10.821 us | 2.697 | 18.364 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 103.320 us | 85.530 us | 1.208 | 17.770 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 61.839 us | 44.682 us | 1.384 | 17.171 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 27.822 us | 11.412 us | 2.438 | 16.411 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 26.416 us | 10.207 us | 2.588 | 16.210 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 59.968 us | 44.819 us | 1.338 | 15.132 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 49.732 us | 36.541 us | 1.361 | 13.194 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 119.941 us | 107.571 us | 1.115 | 12.383 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | shell | 1.195 ms | 1.183 ms | 1.010 | 12.350 us | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 15.170 us | 3.204 us | 4.735 | 11.967 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 21.353 us | 10.222 us | 2.089 | 11.130 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 45.657 us | 34.536 us | 1.322 | 11.129 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | merge | 57.000 us | 46.229 us | 1.233 | 10.770 us | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 26.443 us | 15.778 us | 1.676 | 10.665 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 42.564 us | 32.148 us | 1.324 | 10.407 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 20.961 us | 10.619 us | 1.974 | 10.345 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=0, reason=9 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 14.179 us | 3.870 us | 3.664 | 10.309 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 16.963 us | 6.851 us | 2.476 | 10.113 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 500.780 us | 491.925 us | 1.018 | 8.720 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 17.106 us | 8.536 us | 2.004 | 8.571 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 42.823 us | 34.591 us | 1.238 | 8.241 us | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | shell | 15.165 us | 8.075 us | 1.878 | 7.092 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 15.006 us | 8.512 us | 1.763 | 6.496 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 93.032 us | 86.622 us | 1.074 | 6.436 us | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 73.729 us | 68.394 us | 1.078 | 5.359 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 75.107 us | 69.867 us | 1.075 | 5.238 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 10.367 us | 5.747 us | 1.804 | 4.619 us | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 8.303 us | 4.239 us | 1.959 | 4.065 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | merge | 12.787 us | 8.722 us | 1.466 | 4.064 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=0, reason=9 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 39.543 us | 36.245 us | 1.091 | 3.304 us | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 53.953 us | 50.995 us | 1.058 | 2.963 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 7.066 us | 4.324 us | 1.634 | 2.741 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | intro | 8.761 us | 6.191 us | 1.415 | 2.569 us | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 15.771 us | 13.242 us | 1.191 | 2.533 us | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 11.174 us | 8.662 us | 1.290 | 2.511 us | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 8.040 us | 5.572 us | 1.443 | 2.470 us | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | merge | 7.184 us | 4.770 us | 1.506 | 2.414 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 8.544 us | 6.414 us | 1.332 | 2.132 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 7.439 us | 5.470 us | 1.360 | 1.968 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 3.958 us | 2.079 us | 1.904 | 1.879 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 4.444 us | 2.633 us | 1.688 | 1.812 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 28.751 us | 27.175 us | 1.058 | 1.577 us | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 2.601 us | 1.055 us | 2.466 | 1.546 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 4.080 us | 2.607 us | 1.565 | 1.473 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | merge | 17.643 us | 16.201 us | 1.089 | 1.443 us | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | shell | 3.386 us | 2.071 us | 1.635 | 1.315 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 2.539 us | 1.301 us | 1.951 | 1.237 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 20.608 us | 19.423 us | 1.061 | 1.177 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=0, reason=9 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 4.016 us | 2.957 us | 1.358 | 1.059 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 1.525 us | 770.125 ns | 1.980 | 754.832 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | insertion | 1.879 us | 1.372 us | 1.370 | 507.088 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 1.459 us | 967.835 ns | 1.507 | 490.797 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | insertion | 773.627 ns | 307.973 ns | 2.512 | 465.635 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 957.492 ns | 506.342 ns | 1.891 | 451.137 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | insertion | 663.823 ns | 221.940 ns | 2.991 | 441.886 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | intro | 627.415 ns | 201.741 ns | 3.110 | 425.670 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | intro | 681.465 ns | 274.674 ns | 2.481 | 406.764 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | insertion | 1.656 us | 1.254 us | 1.321 | 402.039 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | insertion | 740.030 ns | 341.815 ns | 2.165 | 398.266 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 4.150 us | 3.752 us | 1.106 | 397.920 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | insertion | 554.939 ns | 158.238 ns | 3.507 | 396.699 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | intro | 1.050 us | 662.746 ns | 1.584 | 387.033 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 553.514 ns | 173.624 ns | 3.188 | 379.873 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | intro | 720.346 ns | 350.193 ns | 2.057 | 370.092 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 1.712 us | 1.344 us | 1.274 | 367.645 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | intro | 1.773 us | 1.485 us | 1.194 | 287.807 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 0 | integers | int8_t | random | 0 | 1 | 1 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 229.456 us | 229.456 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=11 |
| 4 | records | sensor_record | few_unique_2 | 1 | 8 | 12780580985233474033 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 182.936 us | 182.936 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 43 | records | database_record | plateau | 2048 | 48 | 2727476683324678604 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 63 | records | log_record | reverse_first_half | 4096 | 24 | 882992644293677392 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 55.986 us | 55.986 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | shell | 647.475 us | 647.475 us | 1.000 | 0.000 ns | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=8 |
| 76 | records | sensor_record | all_equal | 0 | 8 | 9829787259463779993 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 83 | records | sensor_record | random_tail | 1237 | 8 | 17485252678972227796 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 86 | timestamps | timestamp+sequence | few_unique_2 | 0 | 16 | 1126868243982466986 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 46.207 us | 46.207 us | 1.000 | 0.000 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 71.241 us | 71.241 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |

### Top 100 by dispatcher_overhead_percent

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 0 | integers | int8_t | random | 0 | 1 | 1 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 4 | records | sensor_record | few_unique_2 | 1 | 8 | 12780580985233474033 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | insertion | 554.939 ns | 158.238 ns | 3.507 | 396.699 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | intro | 627.415 ns | 201.741 ns | 3.110 | 425.670 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 553.514 ns | 173.624 ns | 3.188 | 379.873 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | insertion | 773.627 ns | 307.973 ns | 2.512 | 465.635 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | intro | 720.346 ns | 350.193 ns | 2.057 | 370.092 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 8.544 us | 6.414 us | 1.332 | 2.132 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 43 | records | database_record | plateau | 2048 | 48 | 2727476683324678604 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 2.601 us | 1.055 us | 2.466 | 1.546 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 957.492 ns | 506.342 ns | 1.891 | 451.137 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 8.040 us | 5.572 us | 1.443 | 2.470 us | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | intro | 1.050 us | 662.746 ns | 1.584 | 387.033 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 1.525 us | 770.125 ns | 1.980 | 754.832 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 4.080 us | 2.607 us | 1.565 | 1.473 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 4.016 us | 2.957 us | 1.358 | 1.059 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | intro | 8.761 us | 6.191 us | 1.415 | 2.569 us | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | insertion | 1.879 us | 1.372 us | 1.370 | 507.088 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | insertion | 1.656 us | 1.254 us | 1.321 | 402.039 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | shell | 3.386 us | 2.071 us | 1.635 | 1.315 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | intro | 1.773 us | 1.485 us | 1.194 | 287.807 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 34.192 us | 12.701 us | 2.692 | 21.492 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=0, reason=9 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | shell | 15.165 us | 8.075 us | 1.878 | 7.092 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 29.184 us | 10.821 us | 2.697 | 18.364 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 26.416 us | 10.207 us | 2.588 | 16.210 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 39.543 us | 36.245 us | 1.091 | 3.304 us | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 119.941 us | 107.571 us | 1.115 | 12.383 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 20.961 us | 10.619 us | 1.974 | 10.345 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=0, reason=9 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 26.443 us | 15.778 us | 1.676 | 10.665 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 84.800 us | 49.159 us | 1.725 | 35.637 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 15.170 us | 3.204 us | 4.735 | 11.967 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 500.780 us | 491.925 us | 1.018 | 8.720 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 17.106 us | 8.536 us | 2.004 | 8.571 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 49.732 us | 36.541 us | 1.361 | 13.194 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 78.519 us | 10.428 us | 7.530 | 68.092 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 15.771 us | 13.242 us | 1.191 | 2.533 us | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 28.751 us | 27.175 us | 1.058 | 1.577 us | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 42.823 us | 34.591 us | 1.238 | 8.241 us | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 148.414 us | 57.794 us | 2.568 | 90.623 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 59.297 us | 11.344 us | 5.227 | 47.952 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 103.320 us | 85.530 us | 1.208 | 17.770 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | merge | 17.643 us | 16.201 us | 1.089 | 1.443 us | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 229.456 us | 229.456 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=11 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 65.981 us | 40.232 us | 1.640 | 25.749 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 469.720 us | 290.668 us | 1.616 | 179.020 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | intro | 255.312 us | 70.567 us | 3.618 | 184.744 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 203.540 us | 125.720 us | 1.619 | 77.815 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | merge | 57.000 us | 46.229 us | 1.233 | 10.770 us | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 53.953 us | 50.995 us | 1.058 | 2.963 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 104.160 us | 82.930 us | 1.256 | 21.244 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 67.970 us | 15.081 us | 4.507 | 52.890 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 182.936 us | 182.936 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 63 | records | log_record | reverse_first_half | 4096 | 24 | 882992644293677392 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | insertion | 740.030 ns | 341.815 ns | 2.165 | 398.266 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 7.439 us | 5.470 us | 1.360 | 1.968 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 76 | records | sensor_record | all_equal | 0 | 8 | 9829787259463779993 | none | intro | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | intro | 681.465 ns | 274.674 ns | 2.481 | 406.764 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 83 | records | sensor_record | random_tail | 1237 | 8 | 17485252678972227796 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 86 | timestamps | timestamp+sequence | few_unique_2 | 0 | 16 | 1126868243982466986 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | insertion | 663.823 ns | 221.940 ns | 2.991 | 441.886 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 2.539 us | 1.301 us | 1.951 | 1.237 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 3.958 us | 2.079 us | 1.904 | 1.879 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 1.459 us | 967.835 ns | 1.507 | 490.797 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 1.712 us | 1.344 us | 1.274 | 367.645 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 10.367 us | 5.747 us | 1.804 | 4.619 us | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 15.006 us | 8.512 us | 1.763 | 6.496 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 11.174 us | 8.662 us | 1.290 | 2.511 us | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 73.729 us | 68.394 us | 1.078 | 5.359 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 27.822 us | 11.412 us | 2.438 | 16.411 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 93.032 us | 86.622 us | 1.074 | 6.436 us | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 84.542 us | 51.300 us | 1.648 | 33.257 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 16.963 us | 6.851 us | 2.476 | 10.113 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | merge | 12.787 us | 8.722 us | 1.466 | 4.064 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=0, reason=9 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 61.839 us | 44.682 us | 1.384 | 17.171 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 4.150 us | 3.752 us | 1.106 | 397.920 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 83.537 us | 60.976 us | 1.370 | 22.571 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 139.420 us | 89.487 us | 1.558 | 49.937 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 7.066 us | 4.324 us | 1.634 | 2.741 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 45.657 us | 34.536 us | 1.322 | 11.129 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 84.704 us | 15.666 us | 5.407 | 69.038 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | shell | 975.567 us | 904.978 us | 1.078 | 70.967 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=0, reason=8 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 20.608 us | 19.423 us | 1.061 | 1.177 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=0, reason=9 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 21.353 us | 10.222 us | 2.089 | 11.130 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 58.729 us | 37.816 us | 1.553 | 20.923 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 63.922 us | 11.280 us | 5.667 | 52.642 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 14.179 us | 3.870 us | 3.664 | 10.309 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 69.793 us | 45.587 us | 1.531 | 24.218 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 59.968 us | 44.819 us | 1.338 | 15.132 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 30.031 us | 5.012 us | 5.992 | 25.019 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | shell | 647.475 us | 647.475 us | 1.000 | 0.000 ns | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=8 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 8.303 us | 4.239 us | 1.959 | 4.065 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 234.356 us | 79.147 us | 2.961 | 155.221 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | merge | 7.184 us | 4.770 us | 1.506 | 2.414 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 75.107 us | 69.867 us | 1.075 | 5.238 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 71.241 us | 71.241 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | shell | 1.195 ms | 1.183 ms | 1.010 | 12.350 us | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 4.444 us | 2.633 us | 1.688 | 1.812 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 55.986 us | 55.986 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 42.564 us | 32.148 us | 1.324 | 10.407 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 46.207 us | 46.207 us | 1.000 | 0.000 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |

