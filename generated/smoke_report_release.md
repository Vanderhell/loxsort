# Benchmark Report

## Metadata
- git_commit: 5275ea036c40be1223cfb7b0b26a81072a514306
- git_state: dirty
- os: Windows-10-10.0.26100-SP0
- cpu: Intel64 Family 6 Model 79 Stepping 1, GenuineIntel
- compiler: NOT AVAILABLE
- compiler_version: NOT AVAILABLE
- build_configuration: NOT AVAILABLE
- optimization_flags: NOT AVAILABLE
- cmake_options: NOT AVAILABLE
- clock_source: NOT AVAILABLE
- clock_frequency_hz: 10000000
- campaign_command: NOT AVAILABLE
- run_timestamp: 2026-07-16T20:34:22.187650+02:00
- csv_path: generated\smoke_bench_release.csv
- scenario_digest: db2b22da3da511b5c03f3b6fab34761ba06f64aca011126c7365b8c24565ee65
- planned_dataset_count: 100
- executed_dataset_count: 100
- row_count: 700

## Overall Strategy Results

| strategy | total_time | mean_time | median_time | geometric_mean_speedup | p95_time | p99_time | max_time | wins | losses | ties |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| always_insertion | 67.985 ms | 679.855 us | 14.292 us | 0.607x | 2.000 ms | 3.422 ms | 32.848 ms | 25 | 75 | 18 |
| always_shell | 16.650 ms | 166.497 us | 9.814 us | 1.245x | 1.000 ms | 2.000 ms | 2.000 ms | 30 | 70 | 26 |
| always_intro | 28.048 ms | 280.481 us | 9.980 us | 0.906x | 2.000 ms | 2.000 ms | 4.920 ms | 16 | 84 | 17 |
| always_merge_if_available | 4.163 ms | 77.089 us | 4.901 us | 1.392x | 479.260 us | 1.000 ms | 1.000 ms | 29 | 25 | 30 |
| always_cycle_if_available | NA | NA | NA | NA | NA | NA | NA | 0 | 0 | 0 |
| loxsort_dispatcher | 14.687 ms | 157.920 us | 10.038 us | 1.000x | 1.000 ms | 2.000 ms | 2.000 ms | 29 | 64 | 16 |
| oracle_best | 10.840 ms | 108.405 us | 5.821 us | 1.819x | 1.000 ms | 1.000 ms | 2.000 ms | 0 | 0 | 0 |

## LoxSort Quality

- selected_algorithm_total: 14.687 ms
- public_api_total: 5.110 ms
- dispatcher_overhead_total: -9.577 ms
- p50_regret: 1.316
- p95_regret: 6.859
- p99_regret: 306.097
- max_regret: 413.510
- mean_absolute_loss: 104.227 us
- median_absolute_loss: 1.198 us
- total_absolute_loss: 9.693 ms
- dispatcher_overhead_median: 5.190 ns
- dispatcher_overhead_p95: 40.829 ns

## Merge-Eligible Results

- eligible_dataset_count: 51

| strategy | total_time | mean_time | median_time | p95_time | wins | losses | ties | speedup_vs_dispatcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| loxsort_dispatcher | 7.264 ms | 142.430 us | 7.506 us | 1.000 ms | 11 | 40 | 7 | 1.000x |
| always_shell | 4.822 ms | 94.557 us | 4.100 us | 500.075 us | 8 | 43 | 9 | 1.506x |
| always_intro | 8.009 ms | 157.037 us | 9.610 us | 1.000 ms | 5 | 46 | 5 | 0.907x |
| always_merge_if_available | 2.857 ms | 56.017 us | 4.731 us | 141.327 us | 27 | 24 | 28 | 2.543x |
| oracle_best | 2.376 ms | 46.593 us | 3.207 us | 61.288 us | 0 | 0 | 0 | 3.057x |

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
| 100 ns to 1 us | 22 | 1.018 | 3.089 | 4.085 | 306.097 | 4.745 ns | 43.066 us |
| 1 us to 10 us | 37 | 1.569 | 5.985 | 6.859 | 413.510 | 1.527 us | 2.102 ms |
| 10 us to 100 us | 24 | 1.165 | 3.078 | 3.553 | 25.434 | 2.595 us | 1.122 ms |
| 100 us to 1 ms | 8 | 1.963 | 9.996 | 9.996 | 10.995 | 229.378 us | 5.426 ms |
| >= 1 ms | 2 | 1.500 | 1.000 | 1.000 | 2.000 | 499.975 us | 999.950 us |

### Count

| bucket | dataset count | p50 regret | p95 regret | p99 regret | max regret | median absolute loss | total absolute loss |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 0-3 | 4 | 2.333 | 2.666 | 2.666 | 3.000 | 708.315 us | 2.750 ms |
| 4-8 | 9 | 9.996 | 306.097 | 306.097 | 413.510 | 37.029 us | 6.611 ms |
| 9-32 | 33 | 1.587 | 3.089 | 4.085 | 4.605 | 751.213 ns | 35.502 us |
| 33-128 | 30 | 1.145 | 1.711 | 5.072 | 6.859 | 1.065 us | 75.855 us |
| 129-512 | 15 | 1.414 | 5.950 | 5.950 | 5.985 | 11.560 us | 220.892 us |
| 513+ | 2 | 1.000 | 1.000 | 1.000 | 1.000 | 7.143 ns | 14.286 ns |

## Worst Cases

### Top 100 by regret

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | merge | 2.000 ms | 4.837 us | 413.510 | 1.995 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | merge | 37.150 us | 121.367 ns | 306.097 | 37.029 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | merge | 1.000 ms | 39.319 us | 25.434 | 960.730 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | shell | 2.000 ms | 181.901 us | 10.995 | 1.818 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | shell | 2.000 ms | 200.080 us | 9.996 | 1.800 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 14.486 us | 2.112 us | 6.859 | 12.374 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 11.015 us | 1.840 us | 5.985 | 9.174 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 10.038 us | 1.687 us | 5.950 | 8.351 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 15.004 us | 2.958 us | 5.072 | 12.046 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 5.219 us | 1.133 us | 4.605 | 4.086 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 15.767 us | 3.826 us | 4.121 | 11.941 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 2.529 us | 619.206 ns | 4.085 | 1.910 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | insertion | 46.195 us | 13.002 us | 3.553 | 33.192 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 2.527 us | 818.030 ns | 3.089 | 1.709 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 43.504 us | 14.134 us | 3.078 | 29.372 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | shell | 2.000 ms | 666.667 us | 3.000 | 1.333 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 31.114 us | 10.504 us | 2.962 | 20.609 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | merge | 666.767 us | 250.100 us | 2.666 | 416.679 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 6.078 us | 2.432 us | 2.499 | 3.646 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=0, reason=9 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 6.330 us | 2.561 us | 2.472 | 3.770 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 4.989 us | 2.068 us | 2.413 | 2.922 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 5.134 us | 2.205 us | 2.328 | 2.929 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 4.170 us | 1.963 us | 2.124 | 2.207 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 2.895 us | 1.368 us | 2.117 | 1.527 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | shell | 2.000 ms | 1.000 ms | 2.000 | 999.950 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 1.605 us | 821.337 ns | 1.954 | 783.586 ns | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 1.563 us | 811.388 ns | 1.926 | 751.213 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 3.033 us | 1.615 us | 1.878 | 1.419 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 787.869 ns | 423.585 ns | 1.860 | 364.346 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 16.738 us | 9.782 us | 1.711 | 6.954 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 3.653 us | 2.160 us | 1.691 | 1.492 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=0, reason=9 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | merge | 3.047 us | 1.877 us | 1.624 | 1.171 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 16.026 us | 10.060 us | 1.593 | 5.968 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 2.570 us | 1.614 us | 1.592 | 955.867 ns | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | shell | 1.140 us | 718.193 ns | 1.587 | 421.718 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | shell | 2.346 us | 1.495 us | 1.569 | 851.155 ns | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=0, reason=9 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 90.530 us | 61.293 us | 1.477 | 29.243 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 169.005 ns | 115.362 ns | 1.465 | 53.632 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 13.235 us | 9.121 us | 1.451 | 4.115 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 12.663 us | 8.874 us | 1.427 | 3.789 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 42.285 us | 29.905 us | 1.414 | 12.379 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 4.109 us | 2.910 us | 1.412 | 1.198 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=0, reason=9 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 28.869 us | 20.665 us | 1.397 | 8.203 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 6.768 us | 4.965 us | 1.363 | 1.803 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 14.518 us | 10.675 us | 1.360 | 3.846 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 14.907 us | 11.167 us | 1.335 | 3.741 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 1.695 us | 1.288 us | 1.316 | 406.469 ns | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 36.571 us | 28.394 us | 1.288 | 8.171 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 1.513 us | 1.190 us | 1.271 | 322.615 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | intro | 203.370 us | 161.277 us | 1.261 | 42.078 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=8 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 10.047 us | 8.070 us | 1.245 | 1.979 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 20.871 us | 17.392 us | 1.200 | 3.475 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 2.821 us | 2.361 us | 1.195 | 459.668 ns | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 11.543 us | 9.900 us | 1.166 | 1.641 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 10.325 us | 8.986 us | 1.149 | 1.337 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 7.506 us | 6.573 us | 1.142 | 931.285 ns | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 14.920 us | 13.204 us | 1.130 | 1.715 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | shell | 3.519 us | 3.208 us | 1.097 | 312.601 ns | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 6.789 us | 6.217 us | 1.092 | 570.180 ns | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | merge | 12.925 us | 12.321 us | 1.049 | 607.338 ns | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 498.182 ns | 478.561 ns | 1.041 | 19.436 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | intro | 428.360 us | 416.693 us | 1.028 | 11.560 us | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 14.168 us | 13.782 us | 1.028 | 389.523 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 5.567 us | 5.426 us | 1.026 | 143.631 ns | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 202.235 ns | 197.495 ns | 1.024 | 4.801 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | intro | 305.826 ns | 298.950 ns | 1.023 | 6.991 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 19.612 us | 19.190 us | 1.022 | 416.527 ns | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 1.169 us | 1.150 us | 1.016 | 18.260 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 12.561 us | 12.376 us | 1.015 | 187.176 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | shell | 1.065 us | 1.051 us | 1.013 | 13.833 ns | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | intro | 256.884 ns | 253.587 ns | 1.013 | 3.225 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | shell | 346.683 us | 342.572 us | 1.012 | 4.183 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=0, reason=8 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 456.146 ns | 451.630 ns | 1.010 | 4.690 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 13.029 us | 12.952 us | 1.006 | 75.672 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 312.125 ns | 310.263 ns | 1.006 | 2.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | insertion | 596.125 ns | 593.750 ns | 1.004 | 2.182 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 249.128 ns | 248.383 ns | 1.003 | 0.650 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 48.512 us | 48.512 us | 1.000 | 14.286 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 52.531 us | 52.531 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=11 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 122.797 ns | 122.797 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 823.086 ns | 823.086 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 10.840 us | 10.840 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 1.153 us | 1.153 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 109.837 us | 109.837 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | insertion | 262.261 ns | 262.261 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 8.017 us | 8.017 us | 1.000 | 0.000 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 193.013 ns | 193.013 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 26.517 us | 26.517 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 1.000 ms | 1.000 ms | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 14.184 us | 14.184 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 10.737 us | 10.737 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 948.554 ns | 948.554 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 832.376 ns | 832.376 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |

### Top 100 by absolute_loss_ns

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | merge | 2.000 ms | 4.837 us | 413.510 | 1.995 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | shell | 2.000 ms | 181.901 us | 10.995 | 1.818 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | shell | 2.000 ms | 200.080 us | 9.996 | 1.800 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | shell | 2.000 ms | 666.667 us | 3.000 | 1.333 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | shell | 2.000 ms | 1.000 ms | 2.000 | 999.950 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | merge | 1.000 ms | 39.319 us | 25.434 | 960.730 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | merge | 666.767 us | 250.100 us | 2.666 | 416.679 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | intro | 203.370 us | 161.277 us | 1.261 | 42.078 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=8 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | merge | 37.150 us | 121.367 ns | 306.097 | 37.029 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | insertion | 46.195 us | 13.002 us | 3.553 | 33.192 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 43.504 us | 14.134 us | 3.078 | 29.372 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 90.530 us | 61.293 us | 1.477 | 29.243 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 31.114 us | 10.504 us | 2.962 | 20.609 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 42.285 us | 29.905 us | 1.414 | 12.379 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 14.486 us | 2.112 us | 6.859 | 12.374 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 15.004 us | 2.958 us | 5.072 | 12.046 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 15.767 us | 3.826 us | 4.121 | 11.941 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | intro | 428.360 us | 416.693 us | 1.028 | 11.560 us | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 11.015 us | 1.840 us | 5.985 | 9.174 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 10.038 us | 1.687 us | 5.950 | 8.351 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 28.869 us | 20.665 us | 1.397 | 8.203 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 36.571 us | 28.394 us | 1.288 | 8.171 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 16.738 us | 9.782 us | 1.711 | 6.954 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 16.026 us | 10.060 us | 1.593 | 5.968 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | shell | 346.683 us | 342.572 us | 1.012 | 4.183 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=0, reason=8 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 13.235 us | 9.121 us | 1.451 | 4.115 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 5.219 us | 1.133 us | 4.605 | 4.086 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 14.518 us | 10.675 us | 1.360 | 3.846 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 12.663 us | 8.874 us | 1.427 | 3.789 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 6.330 us | 2.561 us | 2.472 | 3.770 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 14.907 us | 11.167 us | 1.335 | 3.741 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 6.078 us | 2.432 us | 2.499 | 3.646 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=0, reason=9 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 20.871 us | 17.392 us | 1.200 | 3.475 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 5.134 us | 2.205 us | 2.328 | 2.929 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 4.989 us | 2.068 us | 2.413 | 2.922 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 4.170 us | 1.963 us | 2.124 | 2.207 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 10.047 us | 8.070 us | 1.245 | 1.979 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 2.529 us | 619.206 ns | 4.085 | 1.910 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 6.768 us | 4.965 us | 1.363 | 1.803 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 14.920 us | 13.204 us | 1.130 | 1.715 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 2.527 us | 818.030 ns | 3.089 | 1.709 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 11.543 us | 9.900 us | 1.166 | 1.641 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 2.895 us | 1.368 us | 2.117 | 1.527 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 3.653 us | 2.160 us | 1.691 | 1.492 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=0, reason=9 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 3.033 us | 1.615 us | 1.878 | 1.419 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 10.325 us | 8.986 us | 1.149 | 1.337 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 4.109 us | 2.910 us | 1.412 | 1.198 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=0, reason=9 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | merge | 3.047 us | 1.877 us | 1.624 | 1.171 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 2.570 us | 1.614 us | 1.592 | 955.867 ns | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 7.506 us | 6.573 us | 1.142 | 931.285 ns | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | shell | 2.346 us | 1.495 us | 1.569 | 851.155 ns | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=0, reason=9 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 1.605 us | 821.337 ns | 1.954 | 783.586 ns | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 1.563 us | 811.388 ns | 1.926 | 751.213 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | merge | 12.925 us | 12.321 us | 1.049 | 607.338 ns | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 6.789 us | 6.217 us | 1.092 | 570.180 ns | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 2.821 us | 2.361 us | 1.195 | 459.668 ns | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | shell | 1.140 us | 718.193 ns | 1.587 | 421.718 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 19.612 us | 19.190 us | 1.022 | 416.527 ns | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 1.695 us | 1.288 us | 1.316 | 406.469 ns | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 14.168 us | 13.782 us | 1.028 | 389.523 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 787.869 ns | 423.585 ns | 1.860 | 364.346 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 1.513 us | 1.190 us | 1.271 | 322.615 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | shell | 3.519 us | 3.208 us | 1.097 | 312.601 ns | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 12.561 us | 12.376 us | 1.015 | 187.176 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 5.567 us | 5.426 us | 1.026 | 143.631 ns | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 13.029 us | 12.952 us | 1.006 | 75.672 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 169.005 ns | 115.362 ns | 1.465 | 53.632 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 498.182 ns | 478.561 ns | 1.041 | 19.436 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 1.169 us | 1.150 us | 1.016 | 18.260 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 48.512 us | 48.512 us | 1.000 | 14.286 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | shell | 1.065 us | 1.051 us | 1.013 | 13.833 ns | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | intro | 305.826 ns | 298.950 ns | 1.023 | 6.991 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 202.235 ns | 197.495 ns | 1.024 | 4.801 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 456.146 ns | 451.630 ns | 1.010 | 4.690 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | intro | 256.884 ns | 253.587 ns | 1.013 | 3.225 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | insertion | 596.125 ns | 593.750 ns | 1.004 | 2.182 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 312.125 ns | 310.263 ns | 1.006 | 2.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 249.128 ns | 248.383 ns | 1.003 | 0.650 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 52.531 us | 52.531 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=11 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 122.797 ns | 122.797 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 823.086 ns | 823.086 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 10.840 us | 10.840 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 1.153 us | 1.153 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 109.837 us | 109.837 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | insertion | 262.261 ns | 262.261 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 8.017 us | 8.017 us | 1.000 | 0.000 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 193.013 ns | 193.013 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 26.517 us | 26.517 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 1.000 ms | 1.000 ms | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 14.184 us | 14.184 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 10.737 us | 10.737 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 948.554 ns | 948.554 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 832.376 ns | 832.376 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |

### Top 100 by dispatcher_overhead_percent

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 193.013 ns | 193.013 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 202.235 ns | 197.495 ns | 1.024 | 4.801 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 456.146 ns | 451.630 ns | 1.010 | 4.690 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 498.182 ns | 478.561 ns | 1.041 | 19.436 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 823.086 ns | 823.086 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | shell | 1.065 us | 1.051 us | 1.013 | 13.833 ns | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 1.153 us | 1.153 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 948.554 ns | 948.554 ns | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 1.169 us | 1.150 us | 1.016 | 18.260 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=2 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 122.797 ns | 122.797 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 169.005 ns | 115.362 ns | 1.465 | 53.632 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | insertion | 596.125 ns | 593.750 ns | 1.004 | 2.182 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 1.605 us | 821.337 ns | 1.954 | 783.586 ns | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 1.695 us | 1.288 us | 1.316 | 406.469 ns | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 2.527 us | 818.030 ns | 3.089 | 1.709 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 2.570 us | 1.614 us | 1.592 | 955.867 ns | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | insertion | 262.261 ns | 262.261 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 249.128 ns | 248.383 ns | 1.003 | 0.650 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 2.895 us | 1.368 us | 2.117 | 1.527 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | intro | 256.884 ns | 253.587 ns | 1.013 | 3.225 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 7.506 us | 6.573 us | 1.142 | 931.285 ns | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | shell | 2.346 us | 1.495 us | 1.569 | 851.155 ns | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=0, reason=9 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 2.529 us | 619.206 ns | 4.085 | 1.910 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=0, reason=2 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 3.653 us | 2.160 us | 1.691 | 1.492 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=0, reason=9 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | merge | 3.047 us | 1.877 us | 1.624 | 1.171 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 312.125 ns | 310.263 ns | 1.006 | 2.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 5.134 us | 2.205 us | 2.328 | 2.929 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 5.219 us | 1.133 us | 4.605 | 4.086 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 2.821 us | 2.361 us | 1.195 | 459.668 ns | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 8.017 us | 8.017 us | 1.000 | 0.000 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | intro | 305.826 ns | 298.950 ns | 1.023 | 6.991 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | shell | 3.519 us | 3.208 us | 1.097 | 312.601 ns | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=0, reason=9 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 6.078 us | 2.432 us | 2.499 | 3.646 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=0, reason=9 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 3.033 us | 1.615 us | 1.878 | 1.419 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 4.170 us | 1.963 us | 2.124 | 2.207 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=9 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 6.789 us | 6.217 us | 1.092 | 570.180 ns | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 787.869 ns | 423.585 ns | 1.860 | 364.346 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 4.109 us | 2.910 us | 1.412 | 1.198 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=0, reason=9 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 832.376 ns | 832.376 ns | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 4.989 us | 2.068 us | 2.413 | 2.922 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 10.840 us | 10.840 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 10.047 us | 8.070 us | 1.245 | 1.979 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 14.486 us | 2.112 us | 6.859 | 12.374 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=2 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 6.768 us | 4.965 us | 1.363 | 1.803 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=9 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 16.026 us | 10.060 us | 1.593 | 5.968 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 1.563 us | 811.388 ns | 1.926 | 751.213 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 5.567 us | 5.426 us | 1.026 | 143.631 ns | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=0, reason=9 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 12.561 us | 12.376 us | 1.015 | 187.176 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 14.184 us | 14.184 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | shell | 1.140 us | 718.193 ns | 1.587 | 421.718 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | merge | 12.925 us | 12.321 us | 1.049 | 607.338 ns | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 6.330 us | 2.561 us | 2.472 | 3.770 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=0, reason=9 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 14.518 us | 10.675 us | 1.360 | 3.846 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 14.907 us | 11.167 us | 1.335 | 3.741 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=0, reason=8 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 14.168 us | 13.782 us | 1.028 | 389.523 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 1.513 us | 1.190 us | 1.271 | 322.615 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 13.029 us | 12.952 us | 1.006 | 75.672 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 11.015 us | 1.840 us | 5.985 | 9.174 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 12.663 us | 8.874 us | 1.427 | 3.789 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=0, reason=8 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 10.737 us | 10.737 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 20.871 us | 17.392 us | 1.200 | 3.475 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 19.612 us | 19.190 us | 1.022 | 416.527 ns | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 14.920 us | 13.204 us | 1.130 | 1.715 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 52.531 us | 52.531 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=0, reason=11 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 10.325 us | 8.986 us | 1.149 | 1.337 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | insertion | 46.195 us | 13.002 us | 3.553 | 33.192 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 28.869 us | 20.665 us | 1.397 | 8.203 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | shell | 346.683 us | 342.572 us | 1.012 | 4.183 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=0, reason=8 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 13.235 us | 9.121 us | 1.451 | 4.115 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 26.517 us | 26.517 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 10.038 us | 1.687 us | 5.950 | 8.351 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 11.543 us | 9.900 us | 1.166 | 1.641 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 36.571 us | 28.394 us | 1.288 | 8.171 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | shell | 2.000 ms | 666.667 us | 3.000 | 1.333 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 15.767 us | 3.826 us | 4.121 | 11.941 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=8 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 109.837 us | 109.837 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=0, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 42.285 us | 29.905 us | 1.414 | 12.379 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=0, reason=8 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 48.512 us | 48.512 us | 1.000 | 14.286 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 15.004 us | 2.958 us | 5.072 | 12.046 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=9 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 90.530 us | 61.293 us | 1.477 | 29.243 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=0, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | intro | 428.360 us | 416.693 us | 1.028 | 11.560 us | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=0, reason=8 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 43.504 us | 14.134 us | 3.078 | 29.372 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 31.114 us | 10.504 us | 2.962 | 20.609 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=0, reason=8 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 16.738 us | 9.782 us | 1.711 | 6.954 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=0, reason=8 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | intro | 203.370 us | 161.277 us | 1.261 | 42.078 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=0, reason=8 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 1.000 ms | 1.000 ms | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | merge | 1.000 ms | 39.319 us | 25.434 | 960.730 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | shell | 2.000 ms | 1.000 ms | 2.000 | 999.950 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | merge | 666.767 us | 250.100 us | 2.666 | 416.679 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | merge | 2.000 ms | 4.837 us | 413.510 | 1.995 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | shell | 2.000 ms | 200.080 us | 9.996 | 1.800 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | shell | 2.000 ms | 181.901 us | 10.995 | 1.818 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | merge | 37.150 us | 121.367 ns | 306.097 | 37.029 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=1 |

