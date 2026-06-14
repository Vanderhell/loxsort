# Benchmark Report

## Metadata
- git_commit: 72229e5863f51ff5952258cee1632e563d906031
- git_state: dirty
- os: Windows-10-10.0.26100-SP0
- cpu: Intel64 Family 6 Model 79 Stepping 1, GenuineIntel
- compiler: MSVC
- compiler_version: NOT AVAILABLE
- build_configuration: Release
- optimization_flags: NOT AVAILABLE
- cmake_options: -DLOXSORT_BUILD_BENCH=ON -DLOXSORT_BUILD_TESTS=ON -DLOXSORT_WARNINGS_AS_ERRORS=ON
- clock_source: QueryPerformanceCounter
- clock_frequency_hz: 10000000
- campaign_command: build_release\Release\loxsort_bench.exe --campaign smoke --seed-base 1 --output generated\smoke_bench_release.csv --checkpoint-interval 10 --timing-target-ms 2 --timing-samples 3
- run_timestamp: 2026-06-14T19:02:41.594604+02:00
- csv_path: generated\smoke_bench_release.csv
- scenario_digest: db2b22da3da511b5c03f3b6fab34761ba06f64aca011126c7365b8c24565ee65
- planned_dataset_count: 100
- executed_dataset_count: 100
- row_count: 700

## Overall Strategy Results

| strategy | total_time | mean_time | median_time | geometric_mean_speedup | p95_time | p99_time | max_time | wins | losses | ties |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| always_insertion | 69.626 ms | 696.262 us | 16.036 us | 0.508x | 2.000 ms | 3.969 ms | 34.759 ms | 27 | 73 | 24 |
| always_shell | 13.948 ms | 139.484 us | 9.447 us | 1.147x | 1.000 ms | 2.000 ms | 2.000 ms | 30 | 70 | 24 |
| always_intro | 27.415 ms | 274.146 us | 10.253 us | 0.781x | 2.000 ms | 2.000 ms | 5.189 ms | 16 | 84 | 18 |
| always_merge_if_available | 6.270 ms | 62.695 us | 809.121 ns | 1.288x | 181.909 us | 2.000 ms | 2.000 ms | 27 | 73 | 73 |
| always_cycle_if_available | 0.000 ns | 0.000 ns | 0.000 ns | NA | 0.000 ns | 0.000 ns | 0.000 ns | 0 | 100 | 100 |
| loxsort_dispatcher | 5.372 ms | 57.762 us | 8.148 us | 1.000x | 333.417 us | 417.160 us | 2.000 ms | 35 | 65 | 20 |
| oracle_best | 10.147 ms | 101.473 us | 4.954 us | 1.663x | 666.733 us | 1.001 ms | 2.000 ms | 0 | 0 | 0 |

## LoxSort Quality

- p50_regret: 1.403
- p95_regret: 5.626
- p99_regret: 28.489
- max_regret: 1085.669
- mean_absolute_loss: 21.039 us
- median_absolute_loss: 915.724 ns
- total_absolute_loss: 2.104 ms
- dispatcher_overhead_median: 5.463 ns
- dispatcher_overhead_p95: NA

## Merge-Eligible Results

- eligible_dataset_count: 54

| strategy | total_time | mean_time | median_time | p95_time | wins | losses | ties | speedup_vs_dispatcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| loxsort_dispatcher | 3.548 ms | 69.569 us | 7.775 us | 133.433 us | 15 | 39 | 7 | 1.000x |
| always_shell | 5.341 ms | 98.903 us | 6.518 us | 666.733 us | 7 | 47 | 6 | 0.664x |
| always_intro | 15.046 ms | 278.622 us | 10.325 us | 1.189 ms | 7 | 47 | 7 | 0.236x |
| always_merge_if_available | 6.270 ms | 116.102 us | 4.718 us | 555.375 us | 27 | 27 | 27 | 0.566x |
| oracle_best | 2.555 ms | 47.323 us | 4.101 us | 224.344 us | 0 | 0 | 0 | 1.388x |

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
| 100 ns to 1 us | 22 | 1.893 | 4.591 | 6.015 | 1085.669 | 324.911 ns | 344.009 us |
| 1 us to 10 us | 36 | 1.700 | 5.933 | 7.680 | 28.489 | 1.856 us | 241.689 us |
| 10 us to 100 us | 27 | 1.127 | 2.897 | 2.959 | 3.550 | 1.980 us | 176.092 us |
| 100 us to 1 ms | 10 | 1.000 | 1.025 | 1.025 | 3.000 | 0.000 ns | 1.342 ms |
| >= 1 ms | 5 | 1.000 | 1.000 | 1.000 | 1.000 | 0.000 ns | 0.000 ns |

### Count

| bucket | dataset count | p50 regret | p95 regret | p99 regret | max regret | median absolute loss | total absolute loss |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 0-3 | 8 | 1.000 | 1.000 | 1.000 | 3.000 | 0.000 ns | 1.333 ms |
| 4-8 | 9 | 1.881 | 28.489 | 28.489 | 1085.669 | 116.408 ns | 463.189 us |
| 9-32 | 33 | 1.905 | 4.069 | 4.591 | 5.265 | 927.678 ns | 49.233 us |
| 33-128 | 30 | 1.196 | 1.604 | 5.611 | 7.680 | 1.379 us | 84.658 us |
| 129-512 | 15 | 1.397 | 5.626 | 5.626 | 5.933 | 9.090 us | 170.848 us |
| 513+ | 5 | 1.000 | 1.000 | 1.000 | 1.050 | 0.000 ns | 2.679 us |

## Worst Cases

### Top 100 by regret

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | merge | 333.417 us | 307.107 ns | 1085.669 | 333.110 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | merge | 133.433 us | 4.684 us | 28.489 | 128.750 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 17.748 us | 2.311 us | 7.680 | 15.437 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | shell | 652.407 ns | 108.463 ns | 6.015 | 543.941 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 12.058 us | 2.032 us | 5.933 | 10.026 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 10.997 us | 1.955 us | 5.626 | 9.042 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 15.839 us | 2.823 us | 5.611 | 13.016 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=9 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 6.125 us | 1.163 us | 5.265 | 4.962 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 3.234 us | 704.370 ns | 4.591 | 2.529 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=1, reason=2 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 15.685 us | 3.843 us | 4.082 | 11.843 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 3.294 us | 809.595 ns | 4.069 | 2.485 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=1, reason=2 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | insertion | 42.785 us | 12.052 us | 3.550 | 30.731 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | shell | 2.000 ms | 666.667 us | 3.000 | 1.333 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 45.309 us | 15.312 us | 2.959 | 29.997 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 31.814 us | 10.982 us | 2.897 | 20.832 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 552.694 ns | 201.126 ns | 2.748 | 351.598 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 5.906 us | 2.185 us | 2.703 | 3.721 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 6.515 us | 2.465 us | 2.643 | 4.050 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=1, reason=9 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 3.697 us | 1.466 us | 2.522 | 2.231 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 6.313 us | 2.533 us | 2.492 | 3.779 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 6.783 us | 2.769 us | 2.450 | 4.015 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 498.828 ns | 210.832 ns | 2.366 | 288.025 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | shell | 992.262 ns | 422.959 ns | 2.346 | 569.276 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 4.816 us | 2.153 us | 2.237 | 2.663 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 3.552 us | 1.718 us | 2.067 | 1.833 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=1, reason=9 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | shell | 3.641 us | 1.763 us | 2.065 | 1.878 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 1.890 us | 943.674 ns | 2.003 | 946.453 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 858.909 ns | 430.315 ns | 1.996 | 428.666 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 1.879 us | 952.096 ns | 1.974 | 927.271 ns | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 875.405 ns | 459.530 ns | 1.905 | 415.823 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 248.552 ns | 132.138 ns | 1.881 | 116.408 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 4.615 us | 2.499 us | 1.847 | 2.117 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=1, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 2.980 us | 1.705 us | 1.748 | 1.275 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 2.237 us | 1.309 us | 1.709 | 927.678 ns | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 2.127 us | 1.257 us | 1.692 | 869.902 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=2 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 792.003 ns | 493.767 ns | 1.604 | 298.224 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | merge | 2.654 us | 1.661 us | 1.598 | 993.182 ns | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=1, reason=9 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | shell | 1.213 us | 760.163 ns | 1.596 | 453.190 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 47.002 us | 29.805 us | 1.577 | 17.196 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 1.395 us | 918.216 ns | 1.519 | 476.734 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 16.839 us | 11.137 us | 1.512 | 5.700 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 1.456 us | 969.853 ns | 1.501 | 486.142 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=2 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 7.775 us | 5.225 us | 1.488 | 2.551 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 892.417 ns | 601.359 ns | 1.484 | 291.188 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 1.839 us | 1.265 us | 1.454 | 574.340 ns | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 14.609 us | 10.103 us | 1.446 | 4.505 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 3.606 us | 2.509 us | 1.437 | 1.097 us | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | intro | 1.634 us | 1.154 us | 1.416 | 479.942 ns | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=1, reason=9 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 4.685 us | 3.320 us | 1.411 | 1.365 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=1, reason=9 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 24.886 us | 17.662 us | 1.409 | 7.225 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 31.971 us | 22.886 us | 1.397 | 9.090 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 13.226 us | 9.682 us | 1.366 | 3.545 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=1, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 93.818 us | 70.540 us | 1.330 | 23.273 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=8 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 16.892 us | 12.856 us | 1.314 | 4.039 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 37.591 us | 28.673 us | 1.311 | 8.908 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 14.887 us | 11.442 us | 1.301 | 3.440 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 11.334 us | 8.712 us | 1.301 | 2.619 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 1.650 us | 1.280 us | 1.289 | 370.286 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | insertion | 320.375 ns | 262.387 ns | 1.221 | 57.908 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | intro | 4.257 us | 3.518 us | 1.210 | 737.615 ns | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 302.939 ns | 250.985 ns | 1.207 | 52.029 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 12.789 us | 10.810 us | 1.183 | 1.980 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | merge | 144.321 ns | 123.141 ns | 1.172 | 21.161 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 156.711 ns | 135.563 ns | 1.156 | 21.200 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | intro | 312.989 ns | 271.456 ns | 1.153 | 41.605 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 11.181 us | 9.731 us | 1.149 | 1.453 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 7.382 us | 6.458 us | 1.143 | 921.274 ns | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 364.649 ns | 322.984 ns | 1.129 | 41.756 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 13.109 us | 11.632 us | 1.127 | 1.475 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 12.861 us | 11.462 us | 1.122 | 1.393 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=1, reason=8 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 16.187 us | 14.466 us | 1.119 | 1.725 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 8.148 us | 7.503 us | 1.086 | 647.625 ns | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=8 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 9.605 us | 8.943 us | 1.074 | 661.960 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | intro | 335.683 ns | 315.492 ns | 1.064 | 20.226 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 15.739 us | 14.834 us | 1.061 | 910.174 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 15.775 us | 14.938 us | 1.056 | 836.743 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=8 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 56.192 us | 53.516 us | 1.050 | 2.679 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=1, reason=11 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 6.543 us | 6.268 us | 1.044 | 275.027 ns | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=1, reason=9 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | intro | 359.950 us | 351.171 us | 1.025 | 8.817 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=1, reason=8 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 13.822 us | 13.672 us | 1.011 | 153.361 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 0 | integers | int8_t | random | 0 | 1 | 1 | none | shell | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | shell | 13.385 us | 13.385 us | 1.000 | 0.000 ns | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 4 | records | sensor_record | few_unique_2 | 1 | 8 | 12780580985233474033 | none | shell | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | insertion | 80.100 us | 80.100 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | insertion | 91.009 us | 91.009 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 47.442 us | 47.442 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=11 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 113.217 us | 113.217 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 43 | records | database_record | plateau | 2048 | 48 | 2727476683324678604 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 26.932 us | 26.932 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 400.080 us | 400.080 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 20.690 us | 20.690 us | 1.000 | 0.000 ns | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 15.743 us | 15.743 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | shell | 417.160 us | 417.160 us | 1.000 | 0.000 ns | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 63 | records | log_record | reverse_first_half | 4096 | 24 | 882992644293677392 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 11.402 us | 11.402 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=1, reason=8 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | shell | 149.800 us | 149.800 us | 1.000 | 0.000 ns | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=8 |
| 76 | records | sensor_record | all_equal | 0 | 8 | 9829787259463779993 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 83 | records | sensor_record | random_tail | 1237 | 8 | 17485252678972227796 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 86 | timestamps | timestamp+sequence | few_unique_2 | 0 | 16 | 1126868243982466986 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | insertion | 333.417 us | 333.417 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |

### Top 100 by absolute_loss_ns

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | shell | 2.000 ms | 666.667 us | 3.000 | 1.333 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | merge | 333.417 us | 307.107 ns | 1085.669 | 333.110 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | merge | 133.433 us | 4.684 us | 28.489 | 128.750 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | insertion | 42.785 us | 12.052 us | 3.550 | 30.731 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 45.309 us | 15.312 us | 2.959 | 29.997 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 93.818 us | 70.540 us | 1.330 | 23.273 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=8 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 31.814 us | 10.982 us | 2.897 | 20.832 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 47.002 us | 29.805 us | 1.577 | 17.196 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 17.748 us | 2.311 us | 7.680 | 15.437 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 15.839 us | 2.823 us | 5.611 | 13.016 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=9 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 15.685 us | 3.843 us | 4.082 | 11.843 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 12.058 us | 2.032 us | 5.933 | 10.026 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 31.971 us | 22.886 us | 1.397 | 9.090 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 10.997 us | 1.955 us | 5.626 | 9.042 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 37.591 us | 28.673 us | 1.311 | 8.908 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | intro | 359.950 us | 351.171 us | 1.025 | 8.817 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=1, reason=8 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 24.886 us | 17.662 us | 1.409 | 7.225 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 16.839 us | 11.137 us | 1.512 | 5.700 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 6.125 us | 1.163 us | 5.265 | 4.962 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 14.609 us | 10.103 us | 1.446 | 4.505 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 6.515 us | 2.465 us | 2.643 | 4.050 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=1, reason=9 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 16.892 us | 12.856 us | 1.314 | 4.039 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 6.783 us | 2.769 us | 2.450 | 4.015 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 6.313 us | 2.533 us | 2.492 | 3.779 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 5.906 us | 2.185 us | 2.703 | 3.721 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 13.226 us | 9.682 us | 1.366 | 3.545 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=1, reason=8 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 14.887 us | 11.442 us | 1.301 | 3.440 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 56.192 us | 53.516 us | 1.050 | 2.679 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=1, reason=11 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 4.816 us | 2.153 us | 2.237 | 2.663 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 11.334 us | 8.712 us | 1.301 | 2.619 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 7.775 us | 5.225 us | 1.488 | 2.551 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 3.234 us | 704.370 ns | 4.591 | 2.529 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=1, reason=2 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 3.294 us | 809.595 ns | 4.069 | 2.485 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=1, reason=2 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 3.697 us | 1.466 us | 2.522 | 2.231 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 4.615 us | 2.499 us | 1.847 | 2.117 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=1, reason=9 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 12.789 us | 10.810 us | 1.183 | 1.980 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | shell | 3.641 us | 1.763 us | 2.065 | 1.878 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 3.552 us | 1.718 us | 2.067 | 1.833 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=1, reason=9 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 16.187 us | 14.466 us | 1.119 | 1.725 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 13.109 us | 11.632 us | 1.127 | 1.475 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 11.181 us | 9.731 us | 1.149 | 1.453 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 12.861 us | 11.462 us | 1.122 | 1.393 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=1, reason=8 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 4.685 us | 3.320 us | 1.411 | 1.365 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=1, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 2.980 us | 1.705 us | 1.748 | 1.275 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 3.606 us | 2.509 us | 1.437 | 1.097 us | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | merge | 2.654 us | 1.661 us | 1.598 | 993.182 ns | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=1, reason=9 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 1.890 us | 943.674 ns | 2.003 | 946.453 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 2.237 us | 1.309 us | 1.709 | 927.678 ns | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 1.879 us | 952.096 ns | 1.974 | 927.271 ns | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 7.382 us | 6.458 us | 1.143 | 921.274 ns | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 15.739 us | 14.834 us | 1.061 | 910.174 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 2.127 us | 1.257 us | 1.692 | 869.902 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=2 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 15.775 us | 14.938 us | 1.056 | 836.743 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=8 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | intro | 4.257 us | 3.518 us | 1.210 | 737.615 ns | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 9.605 us | 8.943 us | 1.074 | 661.960 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 8.148 us | 7.503 us | 1.086 | 647.625 ns | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=8 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 1.839 us | 1.265 us | 1.454 | 574.340 ns | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | shell | 992.262 ns | 422.959 ns | 2.346 | 569.276 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | shell | 652.407 ns | 108.463 ns | 6.015 | 543.941 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 1.456 us | 969.853 ns | 1.501 | 486.142 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=2 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | intro | 1.634 us | 1.154 us | 1.416 | 479.942 ns | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=1, reason=9 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 1.395 us | 918.216 ns | 1.519 | 476.734 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | shell | 1.213 us | 760.163 ns | 1.596 | 453.190 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 858.909 ns | 430.315 ns | 1.996 | 428.666 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 875.405 ns | 459.530 ns | 1.905 | 415.823 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 1.650 us | 1.280 us | 1.289 | 370.286 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 552.694 ns | 201.126 ns | 2.748 | 351.598 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 792.003 ns | 493.767 ns | 1.604 | 298.224 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 892.417 ns | 601.359 ns | 1.484 | 291.188 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 498.828 ns | 210.832 ns | 2.366 | 288.025 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 6.543 us | 6.268 us | 1.044 | 275.027 ns | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=1, reason=9 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 13.822 us | 13.672 us | 1.011 | 153.361 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 248.552 ns | 132.138 ns | 1.881 | 116.408 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | insertion | 320.375 ns | 262.387 ns | 1.221 | 57.908 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 302.939 ns | 250.985 ns | 1.207 | 52.029 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 364.649 ns | 322.984 ns | 1.129 | 41.756 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | intro | 312.989 ns | 271.456 ns | 1.153 | 41.605 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 156.711 ns | 135.563 ns | 1.156 | 21.200 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | merge | 144.321 ns | 123.141 ns | 1.172 | 21.161 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | intro | 335.683 ns | 315.492 ns | 1.064 | 20.226 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 0 | integers | int8_t | random | 0 | 1 | 1 | none | shell | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | shell | 13.385 us | 13.385 us | 1.000 | 0.000 ns | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 4 | records | sensor_record | few_unique_2 | 1 | 8 | 12780580985233474033 | none | shell | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | insertion | 80.100 us | 80.100 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | insertion | 91.009 us | 91.009 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 47.442 us | 47.442 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=11 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 113.217 us | 113.217 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 43 | records | database_record | plateau | 2048 | 48 | 2727476683324678604 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 26.932 us | 26.932 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 400.080 us | 400.080 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 20.690 us | 20.690 us | 1.000 | 0.000 ns | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 15.743 us | 15.743 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | shell | 417.160 us | 417.160 us | 1.000 | 0.000 ns | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 63 | records | log_record | reverse_first_half | 4096 | 24 | 882992644293677392 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 11.402 us | 11.402 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=1, reason=8 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | shell | 149.800 us | 149.800 us | 1.000 | 0.000 ns | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=8 |
| 76 | records | sensor_record | all_equal | 0 | 8 | 9829787259463779993 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 83 | records | sensor_record | random_tail | 1237 | 8 | 17485252678972227796 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 86 | timestamps | timestamp+sequence | few_unique_2 | 0 | 16 | 1126868243982466986 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | insertion | 333.417 us | 333.417 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |

### Top 100 by dispatcher_overhead_percent

| dataset_id | family | type | pattern | count | element_size | seed | selected algorithm | best algorithm | selected time | best time | regret | absolute loss | runtime features |
| --- | --- | --- | --- | ---: | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: | --- |
| 0 | integers | int8_t | random | 0 | 1 | 1 | none | shell | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 4 | records | sensor_record | few_unique_2 | 1 | 8 | 12780580985233474033 | none | shell | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 6 | records | packet_record | organ_pipe | 2 | 16 | 7062109359217508427 | insertion | shell | 2.000 ms | 666.667 us | 3.000 | 1.333 ms | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 51 | integers | uint8_t | random_half | 23 | 1 | 10976298163124065784 | insertion | insertion | 875.405 ns | 459.530 ns | 1.905 | 415.823 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 36 | records | log_record | random | 29 | 24 | 13518755337525782417 | insertion | insertion | 2.127 us | 1.257 us | 1.692 | 869.902 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=2 |
| 26 | strings | char[16] | plateau | 23 | 16 | 3774930279297543808 | intro | intro | 1.634 us | 1.154 us | 1.416 | 479.942 ns | pairs=16, disorder=47, equal_score=191, equal_pairs=12, dir_changes=1, sign_mask=3, fallback=1, reason=9 |
| 18 | strings | char[128] | adversarial | 64 | 128 | 8202072709651854616 | insertion | insertion | 892.417 ns | 601.359 ns | 1.484 | 291.188 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 24 | integers | int32_t | few_unique_8 | 17 | 4 | 5165316782646619193 | intro | intro | 1.839 us | 1.265 us | 1.454 | 574.340 ns | pairs=16, disorder=63, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 20 | records | device_event | reverse | 16 | 64 | 3926296124524852417 | insertion | intro | 312.989 ns | 271.456 ns | 1.153 | 41.605 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 27 | floating_point | float | all_equal | 24 | 4 | 7253676421366409999 | insertion | intro | 3.234 us | 704.370 ns | 4.591 | 2.529 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=1, reason=2 |
| 43 | records | database_record | plateau | 2048 | 48 | 2727476683324678604 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 47 | strings | char[64] | reverse_first_half | 24 | 64 | 8294582464113335601 | insertion | insertion | 498.828 ns | 210.832 ns | 2.366 | 288.025 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 16 | records | database_record | alternating_high_low | 7 | 48 | 15363239376556783629 | insertion | shell | 992.262 ns | 422.959 ns | 2.346 | 569.276 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 14 | strings | char[64] | push_middle | 63 | 64 | 1192271887974234212 | insertion | insertion | 792.003 ns | 493.767 ns | 1.604 | 298.224 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 11 | timestamps | uint64_t timestamps | reverse_first_half | 5 | 8 | 3431997371921333627 | insertion | shell | 248.552 ns | 132.138 ns | 1.881 | 116.408 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 13 | integers | int16_t | random_tail | 6 | 2 | 8946794615170102602 | insertion | insertion | 156.711 ns | 135.563 ns | 1.156 | 21.200 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 39 | records | scheduler_task | few_unique_2 | 7 | 32 | 14164419935356609816 | insertion | insertion | 320.375 ns | 262.387 ns | 1.221 | 57.908 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 1 | floating_point | float | sorted | 33 | 4 | 7806831264735756413 | intro | shell | 3.606 us | 2.509 us | 1.437 | 1.097 us | pairs=16, disorder=95, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 44 | timestamps | uint64_t timestamps | nearly_sorted | 27 | 8 | 10534307948060435016 | intro | merge | 5.906 us | 2.185 us | 2.703 | 3.721 us | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 5 | timestamps | uint32_t ticks | few_unique_8 | 47 | 4 | 2140668176259678829 | intro | intro | 6.543 us | 6.268 us | 1.044 | 275.027 ns | pairs=16, disorder=111, equal_score=31, equal_pairs=2, dir_changes=7, sign_mask=3, fallback=1, reason=9 |
| 8 | integers | uint8_t | plateau | 48 | 1 | 14019601643355240809 | insertion | intro | 17.748 us | 2.311 us | 7.680 | 15.437 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 46 | records | device_event | reverse_second_half | 22 | 64 | 3373141281155506003 | intro | shell | 3.641 us | 1.763 us | 2.065 | 1.878 us | pairs=16, disorder=95, equal_score=111, equal_pairs=7, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 30 | integers | uint32_t | reverse_first_half | 25 | 4 | 6456645978308273991 | intro | merge | 4.615 us | 2.499 us | 1.847 | 2.117 us | pairs=16, disorder=47, equal_score=63, equal_pairs=4, dir_changes=2, sign_mask=3, fallback=1, reason=9 |
| 37 | timestamps | timestamp+priority | ascending_runs | 29 | 16 | 2878842528551987213 | intro | merge | 6.313 us | 2.533 us | 2.492 | 3.779 us | pairs=16, disorder=63, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 45 | characters | lowercase | reverse | 105 | 1 | 12570359049240337800 | shell | shell | 9.605 us | 8.943 us | 1.074 | 661.960 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 15 | floating_point | double | push_front | 256 | 8 | 4671018030043100403 | shell | merge | 47.002 us | 29.805 us | 1.577 | 17.196 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 12 | records | scheduler_task | random_tail | 49 | 32 | 8353438554879163225 | intro | insertion | 15.839 us | 2.823 us | 5.611 | 13.016 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=9 |
| 34 | strings | char[8] | descending_runs | 41 | 8 | 17794531922652784616 | intro | intro | 4.257 us | 3.518 us | 1.210 | 737.615 ns | pairs=16, disorder=127, equal_score=47, equal_pairs=3, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 31 | strings | char[32] | random_tail | 31 | 32 | 17148867324821957217 | intro | merge | 7.775 us | 5.225 us | 1.488 | 2.551 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 40 | floating_point | double | alternating_high_low | 28 | 8 | 16200471036536512600 | intro | merge | 3.552 us | 1.718 us | 2.067 | 1.833 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=1, reason=9 |
| 21 | timestamps | timestamp+sequence | random | 65 | 16 | 11733127389260608829 | shell | shell | 13.109 us | 11.632 us | 1.127 | 1.475 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 19 | integers | uint16_t | adversarial | 15 | 2 | 10238123810831757400 | insertion | intro | 335.683 ns | 315.492 ns | 1.064 | 20.226 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 9 | records | log_record | descending_blocks_4 | 255 | 24 | 7707773957048335826 | shell | insertion | 31.814 us | 10.982 us | 2.897 | 20.832 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 32 | characters | digits | descending_blocks_4 | 128 | 1 | 3623564434070235199 | shell | intro | 11.181 us | 9.731 us | 1.149 | 1.453 us | pairs=16, disorder=175, equal_score=47, equal_pairs=3, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 2 | characters | raw_bytes | reverse | 129 | 1 | 15613662529471512825 | shell | shell | 13.385 us | 13.385 us | 1.000 | 0.000 ns | pairs=16, disorder=143, equal_score=47, equal_pairs=3, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 22 | strings | const char * | few_unique_2 | 257 | 8 | 15211873531329475020 | shell | insertion | 10.997 us | 1.955 us | 5.626 | 9.042 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 23 | records | sensor_record | all_equal | 1024 | 8 | 4571960722355679816 | intro | intro | 47.442 us | 47.442 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=11 |
| 49 | records | sensor_record | push_middle | 219 | 8 | 4018805878986333402 | shell | shell | 26.932 us | 26.932 us | 1.000 | 0.000 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 33 | records | packet_record | descending_runs | 32 | 16 | 12873090739694955018 | intro | merge | 6.515 us | 2.465 us | 2.643 | 4.050 us | pairs=16, disorder=143, equal_score=0, equal_pairs=0, dir_changes=14, sign_mask=3, fallback=1, reason=9 |
| 25 | records | packet_record | plateau | 96 | 16 | 17300233170049265826 | shell | insertion | 7.382 us | 6.458 us | 1.143 | 921.274 ns | pairs=16, disorder=0, equal_score=191, equal_pairs=12, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 48 | integers | int8_t | sawtooth | 114 | 1 | 10330633565293238385 | shell | intro | 16.839 us | 11.137 us | 1.512 | 5.700 us | pairs=16, disorder=0, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 28 | timestamps | timestamp+device_id | stagger | 127 | 16 | 941848735059505016 | shell | merge | 24.886 us | 17.662 us | 1.409 | 7.225 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 35 | integers | int64_t | ascending_runs | 512 | 8 | 1383838950123135784 | shell | merge | 93.818 us | 70.540 us | 1.330 | 23.273 us | pairs=16, disorder=239, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=8 |
| 38 | strings | char[16] | sorted | 93 | 16 | 6357588670620853404 | shell | merge | 37.591 us | 28.673 us | 1.311 | 8.908 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 3 | strings | char[8] | nearly_sorted | 513 | 8 | 4973749720497717621 | intro | intro | 56.192 us | 53.516 us | 1.050 | 2.679 us | pairs=16, disorder=127, equal_score=15, equal_pairs=1, dir_changes=7, sign_mask=3, fallback=1, reason=11 |
| 41 | integers | uint64_t | reverse | 119 | 8 | 2675168145784790582 | shell | merge | 16.892 us | 12.856 us | 1.314 | 4.039 us | pairs=16, disorder=255, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 42 | strings | char[32] | all_equal | 209 | 32 | 13367389492298473808 | shell | insertion | 42.785 us | 12.052 us | 3.550 | 30.731 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 29 | records | sensor_record | random_half | 511 | 8 | 5863289918017334614 | shell | shell | 113.217 us | 113.217 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 17 | characters | ascii | ascending_runs | 8 | 1 | 395241444916098204 | insertion | insertion | 91.009 us | 91.009 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 7 | strings | char[16] | stagger | 3 | 16 | 11983550542175338025 | insertion | insertion | 80.100 us | 80.100 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 10 | strings | char[32] | reverse_first_half | 4 | 32 | 12629215140006165424 | insertion | merge | 144.321 ns | 123.141 ns | 1.172 | 21.161 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 63 | records | log_record | reverse_first_half | 4096 | 24 | 882992644293677392 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 66 | records | scheduler_task | push_front | 24 | 32 | 1528657242124504791 | insertion | insertion | 1.456 us | 969.853 ns | 1.501 | 486.142 ns | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=2 |
| 70 | timestamps | uint32_t ticks | plateau | 18 | 4 | 9981153104691088602 | insertion | intro | 3.294 us | 809.595 ns | 4.069 | 2.485 us | pairs=16, disorder=15, equal_score=239, equal_pairs=15, dir_changes=0, sign_mask=2, fallback=1, reason=2 |
| 53 | records | packet_record | adversarial | 20 | 16 | 11028606700663953806 | intro | merge | 3.697 us | 1.466 us | 2.522 | 2.231 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 76 | records | sensor_record | all_equal | 0 | 8 | 9829787259463779993 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 83 | records | sensor_record | random_tail | 1237 | 8 | 17485252678972227796 | none | merge | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=11 |
| 86 | timestamps | timestamp+sequence | few_unique_2 | 0 | 16 | 1126868243982466986 | none | insertion | NA | NA | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=0, reason=0 |
| 87 | strings | const char * | push_front | 28 | 8 | 4605614386051333177 | insertion | insertion | 552.694 ns | 201.126 ns | 2.748 | 351.598 ns | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 99 | records | device_event | reverse_second_half | 13 | 64 | 8630967818263606180 | insertion | insertion | 1.395 us | 918.216 ns | 1.519 | 476.734 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 90 | floating_point | double | few_unique_8 | 25 | 8 | 2365888902104233762 | intro | shell | 2.237 us | 1.309 us | 1.709 | 927.678 ns | pairs=16, disorder=95, equal_score=63, equal_pairs=4, dir_changes=6, sign_mask=3, fallback=1, reason=9 |
| 80 | records | packet_record | descending_blocks_4 | 20 | 16 | 16839588081141400397 | intro | shell | 2.980 us | 1.705 us | 1.748 | 1.275 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=9 |
| 57 | integers | int16_t | bit_reversal | 12 | 2 | 12267627358785720582 | insertion | insertion | 302.939 ns | 250.985 ns | 1.207 | 52.029 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 67 | strings | char[8] | random_half | 26 | 8 | 6450098425082334389 | intro | merge | 2.654 us | 1.661 us | 1.598 | 993.182 ns | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=11, sign_mask=3, fallback=1, reason=9 |
| 55 | strings | const char * | adversarial | 282 | 8 | 3867440033759024793 | shell | insertion | 12.058 us | 2.032 us | 5.933 | 10.026 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 92 | integers | int8_t | push_front | 78 | 1 | 13651466308908856365 | shell | merge | 11.334 us | 8.712 us | 1.301 | 2.619 us | pairs=16, disorder=0, equal_score=95, equal_pairs=6, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 88 | characters | case_insensitive_ascii | reverse | 120 | 1 | 9527055569009162775 | shell | shell | 13.822 us | 13.672 us | 1.011 | 153.361 ns | pairs=16, disorder=191, equal_score=63, equal_pairs=4, dir_changes=0, sign_mask=2, fallback=1, reason=8 |
| 71 | strings | char[16] | descending_runs | 13 | 16 | 13459899246759954793 | insertion | merge | 1.650 us | 1.280 us | 1.289 | 370.286 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 64 | integers | uint16_t | all_equal | 27 | 2 | 1476348704584616769 | insertion | intro | 6.125 us | 1.163 us | 5.265 | 4.962 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=2 |
| 77 | floating_point | float | random_half | 21 | 4 | 11865838360643682777 | intro | shell | 1.879 us | 952.096 ns | 1.974 | 927.271 ns | pairs=16, disorder=79, equal_score=79, equal_pairs=5, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 91 | strings | char[16] | bit_reversal | 15 | 16 | 11615415207728953581 | insertion | shell | 1.213 us | 760.163 ns | 1.596 | 453.190 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 97 | integers | uint8_t | adversarial | 16 | 1 | 8578659280723718158 | insertion | insertion | 364.649 ns | 322.984 ns | 1.129 | 41.756 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 93 | records | scheduler_task | few_unique_8 | 24 | 32 | 7339638622601951382 | intro | shell | 4.816 us | 2.153 us | 2.237 | 2.663 us | pairs=16, disorder=47, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=9 |
| 78 | timestamps | uint64_t timestamps | ascending_runs | 99 | 8 | 5554010674336777794 | shell | shell | 15.739 us | 14.834 us | 1.061 | 910.174 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=7, sign_mask=3, fallback=1, reason=8 |
| 68 | integers | int32_t | descending_blocks_4 | 86 | 4 | 8486149526262237173 | shell | merge | 14.609 us | 10.103 us | 1.446 | 4.505 us | pairs=16, disorder=175, equal_score=15, equal_pairs=1, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 72 | records | device_event | sorted | 70 | 64 | 2819986437786159589 | shell | merge | 12.861 us | 11.462 us | 1.122 | 1.393 us | pairs=16, disorder=79, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=1, reason=8 |
| 56 | records | sensor_record | nearly_sorted | 31 | 8 | 11674271298494781205 | intro | shell | 6.783 us | 2.769 us | 2.450 | 4.015 us | pairs=16, disorder=79, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=9 |
| 52 | floating_point | float | sawtooth | 53 | 4 | 336385354150270580 | intro | shell | 4.685 us | 3.320 us | 1.411 | 1.365 us | pairs=16, disorder=143, equal_score=31, equal_pairs=2, dir_changes=4, sign_mask=3, fallback=1, reason=9 |
| 85 | records | packet_record | ascending_runs | 111 | 16 | 11766781052956262190 | shell | merge | 14.887 us | 11.442 us | 1.301 | 3.440 us | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 74 | strings | char[32] | random | 81 | 32 | 14105563844590782192 | shell | merge | 13.226 us | 9.682 us | 1.366 | 3.545 us | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=9, sign_mask=3, fallback=1, reason=8 |
| 84 | integers | uint64_t | organ_pipe | 12 | 8 | 18078608739263167173 | insertion | shell | 1.890 us | 943.674 ns | 2.003 | 946.453 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 95 | strings | char[32] | nearly_sorted | 418 | 32 | 178471955697022369 | shell | intro | 359.950 us | 351.171 us | 1.025 | 8.817 us | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=10, sign_mask=3, fallback=1, reason=8 |
| 69 | records | database_record | alternating_high_low | 479 | 48 | 2174321839955332190 | shell | shell | 149.800 us | 149.800 us | 1.000 | 0.000 ns | pairs=16, disorder=111, equal_score=0, equal_pairs=0, dir_changes=5, sign_mask=3, fallback=1, reason=8 |
| 81 | integers | int64_t | nearly_sorted | 127 | 8 | 17432944141432339774 | shell | merge | 16.187 us | 14.466 us | 1.119 | 1.725 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 98 | strings | char[8] | organ_pipe | 105 | 8 | 824136553527849768 | shell | intro | 12.789 us | 10.810 us | 1.183 | 1.980 us | pairs=16, disorder=95, equal_score=31, equal_pairs=2, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 62 | strings | char[32] | stagger | 469 | 32 | 11522905453267472596 | shell | shell | 417.160 us | 417.160 us | 1.000 | 0.000 ns | pairs=16, disorder=175, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 89 | records | log_record | random | 355 | 24 | 329837800924330978 | shell | insertion | 45.309 us | 15.312 us | 2.959 | 29.997 us | pairs=16, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=1, fallback=1, reason=8 |
| 65 | floating_point | double | sorted | 94 | 8 | 9283179969320373181 | shell | shell | 11.402 us | 11.402 us | 1.000 | 0.000 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=6, sign_mask=3, fallback=1, reason=8 |
| 59 | records | packet_record | organ_pipe | 9 | 16 | 12319935896325608604 | insertion | shell | 858.909 ns | 430.315 ns | 1.996 | 428.666 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 94 | timestamps | timestamp+device_id | reverse_second_half | 115 | 16 | 15146469887337707794 | shell | shell | 15.775 us | 14.938 us | 1.056 | 836.743 ns | pairs=16, disorder=127, equal_score=0, equal_pairs=0, dir_changes=1, sign_mask=3, fallback=1, reason=8 |
| 54 | timestamps | uint32_t ticks | random_half | 107 | 4 | 388693891690158602 | shell | shell | 20.690 us | 20.690 us | 1.000 | 0.000 ns | pairs=16, disorder=31, equal_score=15, equal_pairs=1, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 75 | integers | uint32_t | descending_runs | 177 | 4 | 16141614945770684976 | shell | merge | 31.971 us | 22.886 us | 1.397 | 9.090 us | pairs=16, disorder=191, equal_score=0, equal_pairs=0, dir_changes=8, sign_mask=3, fallback=1, reason=8 |
| 58 | strings | char[16] | nearly_sorted | 65 | 16 | 4513104631589852192 | shell | merge | 8.148 us | 7.503 us | 1.086 | 647.625 ns | pairs=16, disorder=95, equal_score=15, equal_pairs=1, dir_changes=9, sign_mask=3, fallback=1, reason=8 |
| 61 | timestamps | timestamp+sequence | adversarial | 102 | 16 | 8044159311198606405 | shell | shell | 15.743 us | 15.743 us | 1.000 | 0.000 ns | pairs=16, disorder=63, equal_score=0, equal_pairs=0, dir_changes=4, sign_mask=3, fallback=1, reason=8 |
| 82 | strings | char[128] | sawtooth | 345 | 128 | 9678421414236471384 | shell | insertion | 15.685 us | 3.843 us | 4.082 | 11.843 us | pairs=16, disorder=0, equal_score=255, equal_pairs=16, dir_changes=0, sign_mask=0, fallback=1, reason=8 |
| 79 | strings | char[64] | all_equal | 4 | 64 | 9032756816405643985 | insertion | merge | 333.417 us | 307.107 ns | 1085.669 | 333.110 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 96 | records | database_record | plateau | 2 | 48 | 7985303220432778781 | insertion | insertion | 333.417 us | 333.417 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 50 | strings | char[128] | push_front | 2 | 128 | 8940247061944163000 | insertion | insertion | 400.080 us | 400.080 us | 1.000 | 0.000 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 60 | characters | uppercase | ascending_runs | 4 | 1 | 15798682038394474795 | insertion | merge | 133.433 us | 4.684 us | 28.489 | 128.750 us | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |
| 73 | characters | alnum | plateau | 8 | 1 | 6298732579855025780 | insertion | shell | 652.407 ns | 108.463 ns | 6.015 | 543.941 ns | pairs=0, disorder=0, equal_score=0, equal_pairs=0, dir_changes=0, sign_mask=0, fallback=1, reason=1 |

