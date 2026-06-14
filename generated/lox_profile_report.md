# LoxSort profile report

## Selected profile
- format_version: 7
- algorithm_mask: 31
- small_count_cutoff: 4
- duplicate_intro_min_count: 16
- duplicate_intro_max_count: 64
- stable_insertion_cutoff: 32
- near_sorted_insertion_max_count: 64
- cycle_min_count: 64
- shell_max_count_elem_small: 256
- shell_max_count_elem_medium: 512
- shell_max_count_elem_large: 1024
- merge_min_count_elem_small: 0
- merge_min_count_elem_medium: 0
- merge_min_count_elem_large: 0
- merge_min_count: 20
- merge_min_data_bytes: 96
- chaotic_merge_min_count: 23
- chaotic_merge_min_data_bytes: 128
- chaotic_merge_data_bytes_max: 512
- chaotic_merge_direction_min: 4
- chaotic_merge_disorder_min: 80
- merge_large_mixed_min_count: 31
- merge_large_mixed_min_data_bytes: 96
- merge_large_mixed_direction_min: 2
- merge_large_mixed_direction_max: 6
- merge_large_mixed_disorder_min: 64
- duplicate_equal_min: 6
- duplicate_disorder_min: 96
- merge_duplicate_equal_min: 0
- merge_duplicate_disorder_min: 0
- sample_pairs: 16
- near_sorted_disorder_max: 12
- near_sorted_direction_changes_max: 4
- cycle_equal_score_max: 32

## Dataset split
- training scenarios: 1564
- validation scenarios: 1564
- test scenarios: 1564
- training seeds: [1, 7, 13, 29]
- validation seeds: [101, 131, 197, 263]
- test seeds: [401, 433, 467, 499]
- training digest: 0b444e046bc553ea2bfdb7b4d68a7d210ada61052400b11949e825ccf6d51f9e
- validation digest: d98768d0fac37e33995149d1f938cdd3c2c032936a3f8f160edda746cd78fd6d
- test digest: 06df8499983f805839ecf0a1a7293212abb89111101fe86fe2d1bea12cbd523c

## Regression guard
- training improved: 215
- training worsened: 224
- training unchanged: 757
- validation improved: 220
- validation worsened: 224
- validation unchanged: 752
- test improved: 218
- test worsened: 231
- test unchanged: 747

## Split regret
### training
- reported scenarios: 1196
- worst cases:
  - ('push_middle', 7, 2, 1, 6, 42, 0, 0, 1, 1, 2): regret=89.500
  - ('all_equal', 16, 4, 1, 15, 0, 255, 15, 0, 0, 0): regret=57.333
  - ('stagger', 15, 4, 7, 14, 145, 0, 0, 1, 1, 11): regret=14.462
  - ('adversarial', 24, 8, 7, 16, 15, 0, 0, 1, 1, 2): regret=9.850
  - ('sorted', 64, 32, 1, 16, 0, 0, 0, 1, 0, 0): regret=7.696

- p50_regret: 1.091
- p95_regret: 2.000
- max_regret: 89.500

### validation
- reported scenarios: 1196
- worst cases:
  - ('push_middle', 63, 32, 263, 16, 15, 0, 0, 1, 1, 2): regret=9.750
  - ('random_half', 47, 32, 101, 16, 47, 0, 0, 1, 1, 4): regret=4.896
  - ('nearly_sorted', 32, 16, 101, 16, 127, 0, 0, 1, 1, 15): regret=3.571
  - ('nearly_sorted', 32, 16, 131, 16, 127, 0, 0, 1, 1, 15): regret=3.571
  - ('nearly_sorted', 32, 16, 197, 16, 127, 0, 0, 1, 1, 15): regret=3.571

- p50_regret: 1.083
- p95_regret: 2.000
- max_regret: 9.750

### test
- reported scenarios: 1196
- worst cases:
  - ('ascending_runs', 4, 1, 433, 3, 0, 0, 0, 1, 0, 0): regret=171.000
  - ('sorted', 23, 8, 467, 16, 0, 0, 0, 1, 0, 0): regret=8.857
  - ('reverse_second_half', 31, 16, 499, 16, 127, 0, 0, 1, 1, 1): regret=7.875
  - ('stagger', 23, 8, 467, 16, 111, 0, 0, 1, 1, 12): regret=5.594
  - ('descending_blocks_4', 48, 32, 433, 16, 207, 0, 0, 1, 1, 6): regret=5.308

- p50_regret: 1.147
- p95_regret: 2.038
- max_regret: 171.000

## Worst validation cases
- pattern: push_middle
  count: 63
  element_size: 32
  features: dataset_seed=263, sampled_pair_count=16, disorder_score=15, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=2
  selected_algorithm: shell
  best_algorithm: insertion
  regret: 9.750
- pattern: random_half
  count: 47
  element_size: 32
  features: dataset_seed=101, sampled_pair_count=16, disorder_score=47, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=4
  selected_algorithm: shell
  best_algorithm: merge
  regret: 4.896
- pattern: nearly_sorted
  count: 32
  element_size: 16
  features: dataset_seed=101, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=15
  selected_algorithm: merge
  best_algorithm: insertion
  regret: 3.571
- pattern: nearly_sorted
  count: 32
  element_size: 16
  features: dataset_seed=131, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=15
  selected_algorithm: merge
  best_algorithm: insertion
  regret: 3.571
- pattern: nearly_sorted
  count: 32
  element_size: 16
  features: dataset_seed=197, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=15
  selected_algorithm: merge
  best_algorithm: insertion
  regret: 3.571
- pattern: nearly_sorted
  count: 32
  element_size: 16
  features: dataset_seed=263, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=15
  selected_algorithm: merge
  best_algorithm: insertion
  regret: 3.571
- pattern: bit_reversal
  count: 16
  element_size: 4
  features: dataset_seed=101, sampled_pair_count=15, disorder_score=119, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=14
  selected_algorithm: shell
  best_algorithm: merge
  regret: 3.364
- pattern: nearly_sorted
  count: 7
  element_size: 2
  features: dataset_seed=131, sampled_pair_count=6, disorder_score=85, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=3
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.000
- pattern: nearly_sorted
  count: 7
  element_size: 2
  features: dataset_seed=263, sampled_pair_count=6, disorder_score=85, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=3
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.000
- pattern: random_tail
  count: 8
  element_size: 2
  features: dataset_seed=197, sampled_pair_count=7, disorder_score=36, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=1
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.000

## Shell vs merge validation table
| pattern | count | element_size | seed | disorder_score | equal_pair_count | has_less | has_greater | direction_changes | generic_selected | candidate_selected | best_algorithm | generic_regret | candidate_regret | absolute_time_difference | shell_time | merge_time | second_best_algorithm | second_best_time | regret | matched_merge_rule |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| random_half | 47 | 32 | 101 | 47 | 0 | 1 | 1 | 4 | shell | shell | merge | 4.896 | 4.896 | 0 | 235 | 48 | merge | 48 | 4.896 |  |
| bit_reversal | 16 | 4 | 101 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 1.909 | 3.364 | 16 | 37 | 11 | merge | 11 | 3.364 |  |
| reverse_first_half | 32 | 16 | 101 | 127 | 0 | 1 | 1 | 1 | shell | shell | merge | 2.825 | 2.825 | 0 | 113 | 40 | merge | 40 | 2.825 |  |
| bit_reversal | 15 | 4 | 131 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 2.200 | 2.300 | 1 | 23 | 10 | merge | 10 | 2.300 |  |
| bit_reversal | 15 | 4 | 197 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 1.818 | 2.273 | 5 | 25 | 11 | merge | 11 | 2.273 |  |
| bit_reversal | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 2.000 | 2.273 | 3 | 25 | 11 | merge | 11 | 2.273 |  |
| bit_reversal | 16 | 4 | 197 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 2.091 | 2.273 | 2 | 25 | 11 | merge | 11 | 2.273 |  |
| reverse | 8 | 2 | 101 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 3.000 | 2.250 | 3 | 9 | 4 | merge | 4 | 2.250 |  |
| reverse | 8 | 2 | 131 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 3.000 | 2.250 | 3 | 9 | 4 | merge | 4 | 2.250 |  |
| reverse | 8 | 2 | 263 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 3.250 | 2.250 | 4 | 9 | 4 | merge | 4 | 2.250 |  |
| alternating_high_low | 32 | 16 | 101 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 2.192 | 2.192 | 0 | 57 | 26 | merge | 26 | 2.192 |  |
| alternating_high_low | 48 | 32 | 101 | 111 | 0 | 1 | 1 | 14 | shell | shell | merge | 2.190 | 2.190 | 0 | 92 | 42 | merge | 42 | 2.190 | large_mixed |
| alternating_high_low | 48 | 32 | 197 | 111 | 0 | 1 | 1 | 14 | shell | shell | merge | 2.190 | 2.190 | 0 | 92 | 42 | merge | 42 | 2.190 | large_mixed |
| bit_reversal | 15 | 4 | 101 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 2.000 | 2.182 | 2 | 24 | 11 | merge | 11 | 2.182 |  |
| bit_reversal | 15 | 4 | 263 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 1.818 | 2.182 | 4 | 24 | 11 | merge | 11 | 2.182 |  |
| alternating_high_low | 48 | 32 | 263 | 111 | 0 | 1 | 1 | 14 | shell | shell | merge | 2.163 | 2.163 | 0 | 93 | 43 | merge | 43 | 2.163 | large_mixed |
| alternating_high_low | 47 | 32 | 131 | 127 | 0 | 1 | 1 | 13 | shell | shell | merge | 2.133 | 2.133 | 0 | 96 | 45 | merge | 45 | 2.133 | large_mixed |
| alternating_high_low | 47 | 32 | 263 | 127 | 0 | 1 | 1 | 13 | shell | shell | merge | 2.133 | 2.133 | 0 | 96 | 45 | merge | 45 | 2.133 | large_mixed |
| random_half | 64 | 32 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 8.361 | 2.131 | 380 | 130 | 61 | merge | 61 | 2.131 |  |
| alternating_high_low | 47 | 32 | 101 | 127 | 0 | 1 | 1 | 13 | shell | shell | merge | 2.095 | 2.095 | 0 | 88 | 42 | merge | 42 | 2.095 | large_mixed |
| bit_reversal | 32 | 16 | 197 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 2.074 | 2.074 | 0 | 56 | 27 | merge | 27 | 2.074 |  |
| alternating_high_low | 32 | 16 | 197 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 2.036 | 2.036 | 0 | 57 | 28 | merge | 28 | 2.036 |  |
| alternating_high_low | 32 | 16 | 263 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 2.036 | 2.036 | 0 | 57 | 28 | merge | 28 | 2.036 |  |
| bit_reversal | 16 | 4 | 263 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 1.833 | 2.000 | 2 | 24 | 12 | merge | 12 | 2.000 |  |
| bit_reversal | 32 | 16 | 263 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 2.000 | 2.000 | 0 | 52 | 26 | merge | 26 | 2.000 |  |
| bit_reversal | 32 | 16 | 101 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.966 | 1.966 | 0 | 57 | 29 | merge | 29 | 1.966 |  |
| alternating_high_low | 48 | 32 | 131 | 111 | 0 | 1 | 1 | 14 | shell | shell | merge | 1.957 | 1.957 | 0 | 90 | 46 | merge | 46 | 1.957 | large_mixed |
| alternating_high_low | 47 | 32 | 197 | 127 | 0 | 1 | 1 | 13 | shell | shell | merge | 1.956 | 1.956 | 0 | 88 | 45 | merge | 45 | 1.956 | large_mixed |
| bit_reversal | 32 | 16 | 131 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.926 | 1.926 | 0 | 52 | 27 | merge | 27 | 1.926 |  |
| ascending_runs | 64 | 32 | 101 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.925 | 1.925 | 0 | 102 | 53 | merge | 53 | 1.925 |  |
| ascending_runs | 47 | 32 | 131 | 63 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.919 | 1.919 | 0 | 71 | 37 | merge | 37 | 1.919 |  |
| ascending_runs | 47 | 32 | 197 | 63 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.919 | 1.919 | 0 | 71 | 37 | merge | 37 | 1.919 |  |
| random_half | 63 | 32 | 263 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 7.459 | 1.918 | 338 | 117 | 61 | merge | 61 | 1.918 |  |
| alternating_high_low | 15 | 4 | 197 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 2.200 | 1.900 | 3 | 19 | 10 | merge | 10 | 1.900 |  |
| ascending_runs | 47 | 32 | 101 | 63 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.892 | 1.892 | 0 | 70 | 37 | merge | 37 | 1.892 |  |
| bit_reversal | 47 | 32 | 197 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.891 | 1.891 | 0 | 87 | 46 | merge | 46 | 1.891 | large_mixed |
| random_half | 31 | 16 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 5.042 | 1.875 | 76 | 45 | 24 | merge | 24 | 1.875 |  |
| alternating_high_low | 32 | 16 | 131 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.857 | 1.857 | 0 | 52 | 28 | merge | 28 | 1.857 |  |
| ascending_runs | 48 | 32 | 101 | 47 | 0 | 1 | 1 | 6 | merge | shell | merge | 1.000 | 1.854 | 35 | 76 | 41 | merge | 41 | 1.854 |  |
| ascending_runs | 48 | 32 | 197 | 47 | 0 | 1 | 1 | 6 | merge | shell | merge | 1.000 | 1.854 | 35 | 76 | 41 | merge | 41 | 1.854 |  |
| ascending_runs | 48 | 32 | 263 | 47 | 0 | 1 | 1 | 6 | merge | shell | merge | 1.000 | 1.854 | 35 | 76 | 41 | merge | 41 | 1.854 |  |
| bit_reversal | 47 | 32 | 101 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.851 | 1.851 | 0 | 87 | 47 | merge | 47 | 1.851 | large_mixed |
| bit_reversal | 47 | 32 | 131 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.851 | 1.851 | 0 | 87 | 47 | merge | 47 | 1.851 | large_mixed |
| bit_reversal | 48 | 32 | 197 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.837 | 1.837 | 0 | 90 | 49 | merge | 49 | 1.837 | large_mixed |
| ascending_runs | 48 | 32 | 131 | 47 | 0 | 1 | 1 | 6 | merge | shell | merge | 1.000 | 1.829 | 34 | 75 | 41 | merge | 41 | 1.829 |  |
| reverse | 47 | 32 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.825 | 1.825 | 0 | 73 | 40 | merge | 40 | 1.825 |  |
| reverse | 24 | 8 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.824 | 1.824 | 0 | 31 | 17 | merge | 17 | 1.824 |  |
| ascending_runs | 24 | 8 | 197 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.824 | 1.824 | 0 | 31 | 17 | merge | 17 | 1.824 |  |
| random | 63 | 32 | 197 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.819 | 1.819 | 0 | 131 | 72 | merge | 72 | 1.819 | large_mixed |
| random | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 12 | insertion | shell | merge | 1.818 | 1.818 | 0 | 20 | 11 | merge | 11 | 1.818 |  |
| ascending_runs | 64 | 32 | 131 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.808 | 1.808 | 0 | 94 | 52 | merge | 52 | 1.808 |  |
| ascending_runs | 64 | 32 | 263 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.808 | 1.808 | 0 | 94 | 52 | merge | 52 | 1.808 |  |
| random | 48 | 32 | 263 | 127 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.804 | 1.804 | 0 | 92 | 51 | merge | 51 | 1.804 | large_mixed |
| bit_reversal | 64 | 32 | 131 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.803 | 1.803 | 0 | 110 | 61 | merge | 61 | 1.803 |  |
| bit_reversal | 64 | 32 | 263 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.803 | 1.803 | 0 | 110 | 61 | merge | 61 | 1.803 |  |
| alternating_high_low | 15 | 4 | 101 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 2.400 | 1.800 | 6 | 18 | 10 | merge | 10 | 1.800 |  |
| random | 64 | 32 | 101 | 143 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.800 | 1.800 | 0 | 135 | 75 | merge | 75 | 1.800 | large_mixed |
| reverse | 63 | 32 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.788 | 1.788 | 0 | 93 | 52 | merge | 52 | 1.788 |  |
| ascending_runs | 64 | 32 | 197 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.788 | 1.788 | 0 | 93 | 52 | merge | 52 | 1.788 |  |
| ascending_runs | 47 | 32 | 263 | 63 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.757 | 1.757 | 0 | 65 | 37 | merge | 37 | 1.757 |  |
| random | 8 | 2 | 263 | 72 | 0 | 1 | 1 | 3 | insertion | shell | merge | 2.000 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_blocks_4 | 8 | 2 | 101 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_blocks_4 | 8 | 2 | 131 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_blocks_4 | 8 | 2 | 197 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_blocks_4 | 8 | 2 | 263 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_runs | 8 | 2 | 101 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_runs | 8 | 2 | 131 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| descending_runs | 8 | 2 | 197 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.750 | 1 | 7 | 4 | merge | 4 | 1.750 |  |
| alternating_high_low | 8 | 2 | 197 | 109 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.750 | 1.750 | 0 | 7 | 4 | merge | 4 | 1.750 |  |
| bit_reversal | 47 | 32 | 263 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.740 | 1.740 | 0 | 87 | 50 | merge | 50 | 1.740 | large_mixed |
| random | 48 | 32 | 101 | 95 | 0 | 1 | 1 | 10 | shell | shell | merge | 1.736 | 1.736 | 0 | 92 | 53 | merge | 53 | 1.736 | large_mixed |
| random | 16 | 4 | 263 | 119 | 0 | 1 | 1 | 7 | insertion | shell | merge | 2.727 | 1.727 | 11 | 19 | 11 | merge | 11 | 1.727 |  |
| reverse | 31 | 16 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.727 | 1.727 | 0 | 38 | 22 | merge | 22 | 1.727 |  |
| stagger | 47 | 32 | 263 | 111 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.721 | 1.721 | 0 | 74 | 43 | merge | 43 | 1.721 | large_mixed |
| bit_reversal | 48 | 32 | 131 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.708 | 1.708 | 0 | 82 | 48 | merge | 48 | 1.708 | large_mixed |
| bit_reversal | 64 | 32 | 101 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.708 | 1.708 | 0 | 111 | 65 | merge | 65 | 1.708 |  |
| random | 64 | 32 | 131 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.707 | 1.707 | 0 | 128 | 75 | merge | 75 | 1.707 | large_mixed |
| ascending_runs | 23 | 8 | 131 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.706 | 1.706 | 0 | 29 | 17 | merge | 17 | 1.706 |  |
| ascending_runs | 23 | 8 | 263 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.706 | 1.706 | 0 | 29 | 17 | merge | 17 | 1.706 |  |
| ascending_runs | 24 | 8 | 101 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.706 | 1.706 | 0 | 29 | 17 | merge | 17 | 1.706 |  |
| random_half | 16 | 4 | 131 | 68 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.200 | 1.700 | 15 | 17 | 10 | merge | 10 | 1.700 |  |
| stagger | 47 | 32 | 131 | 111 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.698 | 1.698 | 0 | 73 | 43 | merge | 43 | 1.698 | large_mixed |
| ascending_runs | 23 | 8 | 197 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.688 | 1.688 | 0 | 27 | 16 | merge | 16 | 1.688 |  |
| bit_reversal | 48 | 32 | 263 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.688 | 1.688 | 0 | 81 | 48 | merge | 48 | 1.688 | large_mixed |
| reverse | 31 | 16 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.682 | 1.682 | 0 | 37 | 22 | merge | 22 | 1.682 |  |
| bit_reversal | 48 | 32 | 101 | 143 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.679 | 1.679 | 0 | 89 | 53 | merge | 53 | 1.679 | large_mixed |
| reverse | 64 | 32 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.679 | 1.679 | 0 | 89 | 53 | merge | 53 | 1.679 |  |
| reverse | 47 | 32 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.675 | 1.675 | 0 | 67 | 40 | merge | 40 | 1.675 |  |
| reverse | 47 | 32 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.675 | 1.675 | 0 | 67 | 40 | merge | 40 | 1.675 |  |
| reverse | 47 | 32 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.675 | 1.675 | 0 | 67 | 40 | merge | 40 | 1.675 |  |
| reverse | 63 | 32 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.673 | 1.673 | 0 | 92 | 55 | merge | 55 | 1.673 |  |
| random_half | 63 | 32 | 101 | 63 | 0 | 1 | 1 | 6 | merge | shell | merge | 1.000 | 1.672 | 43 | 107 | 64 | merge | 64 | 1.672 |  |
| reverse | 7 | 2 | 101 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 3.333 | 1.667 | 5 | 5 | 3 | merge | 3 | 1.667 |  |
| reverse | 7 | 2 | 197 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 3.333 | 1.667 | 5 | 5 | 3 | merge | 3 | 1.667 |  |
| descending_runs | 7 | 2 | 263 | 212 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.333 | 1.667 | 1 | 5 | 3 | merge | 3 | 1.667 |  |
| alternating_high_low | 7 | 2 | 131 | 127 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.667 | 1.667 | 0 | 5 | 3 | merge | 3 | 1.667 |  |
| alternating_high_low | 16 | 4 | 263 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 2.250 | 1.667 | 7 | 20 | 12 | merge | 12 | 1.667 |  |
| random_half | 31 | 16 | 101 | 63 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.667 | 1.667 | 0 | 40 | 24 | merge | 24 | 1.667 |  |
| random_half | 32 | 16 | 263 | 47 | 0 | 1 | 1 | 4 | shell | shell | merge | 1.667 | 1.667 | 0 | 45 | 27 | merge | 27 | 1.667 |  |
| reverse | 63 | 32 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.667 | 1.667 | 0 | 90 | 54 | merge | 54 | 1.667 |  |
| bit_reversal | 64 | 32 | 197 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.656 | 1.656 | 0 | 101 | 61 | merge | 61 | 1.656 |  |
| reverse | 24 | 8 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.647 | 1.647 | 0 | 28 | 17 | merge | 17 | 1.647 |  |
| random | 63 | 32 | 101 | 111 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.637 | 1.637 | 0 | 131 | 80 | merge | 80 | 1.637 | large_mixed |
| alternating_high_low | 15 | 4 | 263 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 2.182 | 1.636 | 6 | 18 | 11 | merge | 11 | 1.636 |  |
| random_half | 16 | 4 | 263 | 51 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.091 | 1.636 | 16 | 18 | 11 | merge | 11 | 1.636 |  |
| random | 63 | 32 | 131 | 143 | 0 | 1 | 1 | 9 | shell | shell | merge | 1.636 | 1.636 | 0 | 126 | 77 | merge | 77 | 1.636 | large_mixed |
| random_half | 31 | 16 | 197 | 79 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.630 | 1.630 | 0 | 44 | 27 | merge | 27 | 1.630 | large_mixed |
| alternating_high_low | 64 | 32 | 131 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.627 | 1.627 | 0 | 96 | 59 | merge | 59 | 1.627 |  |
| alternating_high_low | 64 | 32 | 263 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.627 | 1.627 | 0 | 96 | 59 | merge | 59 | 1.627 |  |
| alternating_high_low | 64 | 32 | 197 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.610 | 1.610 | 0 | 95 | 59 | merge | 59 | 1.610 |  |
| stagger | 47 | 32 | 197 | 111 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.609 | 1.609 | 0 | 74 | 46 | merge | 46 | 1.609 | large_mixed |
| stagger | 47 | 32 | 101 | 111 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.605 | 1.605 | 0 | 69 | 43 | merge | 43 | 1.605 | large_mixed |
| random | 64 | 32 | 263 | 79 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.602 | 1.602 | 0 | 133 | 83 | merge | 83 | 1.602 | large_mixed |
| reverse | 8 | 2 | 197 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 2.600 | 1.600 | 5 | 8 | 5 | merge | 5 | 1.600 |  |
| stagger | 8 | 2 | 101 | 182 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.400 | 1.600 | 1 | 8 | 5 | merge | 5 | 1.600 |  |
| stagger | 8 | 2 | 131 | 182 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.400 | 1.600 | 1 | 8 | 5 | merge | 5 | 1.600 |  |
| stagger | 8 | 2 | 197 | 182 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.400 | 1.600 | 1 | 8 | 5 | merge | 5 | 1.600 |  |
| ascending_runs | 16 | 4 | 263 | 51 | 0 | 1 | 1 | 6 | insertion | shell | merge | 4.000 | 1.600 | 24 | 16 | 10 | merge | 10 | 1.600 |  |
| random | 47 | 32 | 101 | 95 | 0 | 1 | 1 | 10 | shell | shell | merge | 1.600 | 1.600 | 0 | 88 | 55 | merge | 55 | 1.600 | large_mixed |
| random | 47 | 32 | 131 | 63 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.600 | 1.600 | 0 | 88 | 55 | merge | 55 | 1.600 |  |
| reverse | 32 | 16 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.591 | 1.591 | 0 | 35 | 22 | merge | 22 | 1.591 |  |
| alternating_high_low | 15 | 4 | 131 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 2.000 | 1.583 | 5 | 19 | 12 | merge | 12 | 1.583 |  |
| alternating_high_low | 16 | 4 | 101 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 2.167 | 1.583 | 7 | 19 | 12 | merge | 12 | 1.583 |  |
| alternating_high_low | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 2.250 | 1.583 | 8 | 19 | 12 | merge | 12 | 1.583 |  |
| alternating_high_low | 16 | 4 | 197 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 2.083 | 1.583 | 6 | 19 | 12 | merge | 12 | 1.583 |  |
| reverse | 24 | 8 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.579 | 1.579 | 0 | 30 | 19 | merge | 19 | 1.579 |  |
| random | 23 | 8 | 197 | 79 | 0 | 1 | 1 | 9 | shell | shell | merge | 1.571 | 1.571 | 0 | 33 | 21 | merge | 21 | 1.571 |  |
| reverse | 64 | 32 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.561 | 1.561 | 0 | 89 | 57 | merge | 57 | 1.561 |  |
| random_half | 47 | 32 | 131 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 5.860 | 1.558 | 185 | 67 | 43 | merge | 43 | 1.558 |  |
| descending_runs | 15 | 4 | 131 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.333 | 1.556 | 2 | 14 | 9 | merge | 9 | 1.556 |  |
| ascending_runs | 23 | 8 | 101 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.556 | 1.556 | 0 | 28 | 18 | merge | 18 | 1.556 |  |
| ascending_runs | 24 | 8 | 131 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.556 | 1.556 | 0 | 28 | 18 | merge | 18 | 1.556 |  |
| alternating_high_low | 64 | 32 | 101 | 239 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.554 | 1.554 | 0 | 101 | 65 | merge | 65 | 1.554 |  |
| random_half | 24 | 8 | 197 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.550 | 1.550 | 0 | 31 | 20 | merge | 20 | 1.550 |  |
| reverse | 48 | 32 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.550 | 1.550 | 0 | 62 | 40 | merge | 40 | 1.550 |  |
| reverse | 48 | 32 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.541 | 1.541 | 0 | 57 | 37 | merge | 37 | 1.541 |  |
| few_unique_2 | 15 | 4 | 197 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 1.154 | 1.538 | 5 | 20 | 13 | merge | 13 | 1.538 |  |
| reverse | 64 | 32 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.528 | 1.528 | 0 | 81 | 53 | merge | 53 | 1.528 |  |
| random | 48 | 32 | 131 | 79 | 0 | 1 | 1 | 9 | shell | shell | merge | 1.527 | 1.527 | 0 | 84 | 55 | merge | 55 | 1.527 | large_mixed |
| reverse | 24 | 8 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.526 | 1.526 | 0 | 29 | 19 | merge | 19 | 1.526 |  |
| random_half | 48 | 32 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 6.000 | 1.523 | 197 | 67 | 44 | merge | 44 | 1.523 |  |
| ascending_runs | 31 | 16 | 101 | 63 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.522 | 1.522 | 0 | 35 | 23 | merge | 23 | 1.522 |  |
| ascending_runs | 31 | 16 | 263 | 63 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.522 | 1.522 | 0 | 35 | 23 | merge | 23 | 1.522 |  |
| reverse | 64 | 32 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.509 | 1.509 | 0 | 86 | 57 | merge | 57 | 1.509 |  |
| random | 7 | 2 | 101 | 170 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.750 | 1.500 | 1 | 6 | 4 | merge | 4 | 1.500 |  |
| few_unique_8 | 7 | 2 | 131 | 170 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.750 | 1.500 | 1 | 6 | 4 | merge | 4 | 1.500 |  |
| stagger | 7 | 2 | 101 | 170 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.250 | 1.500 | 1 | 6 | 4 | merge | 4 | 1.500 |  |
| stagger | 7 | 2 | 131 | 170 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.250 | 1.500 | 1 | 6 | 4 | merge | 4 | 1.500 |  |
| stagger | 7 | 2 | 197 | 170 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.250 | 1.500 | 1 | 6 | 4 | merge | 4 | 1.500 |  |
| descending_runs | 8 | 2 | 263 | 218 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.500 | 0 | 6 | 4 | merge | 4 | 1.500 |  |
| random_half | 15 | 4 | 263 | 36 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.900 | 1.500 | 14 | 15 | 10 | merge | 10 | 1.500 |  |
| ascending_runs | 15 | 4 | 197 | 54 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.900 | 1.500 | 24 | 15 | 10 | merge | 10 | 1.500 |  |
| descending_blocks_4 | 16 | 4 | 197 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.200 | 1.500 | 3 | 15 | 10 | merge | 10 | 1.500 |  |
| descending_blocks_4 | 16 | 4 | 263 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.200 | 1.500 | 3 | 15 | 10 | merge | 10 | 1.500 |  |
| ascending_runs | 16 | 4 | 101 | 51 | 0 | 1 | 1 | 6 | insertion | shell | merge | 4.100 | 1.500 | 26 | 15 | 10 | merge | 10 | 1.500 |  |
| ascending_runs | 16 | 4 | 197 | 51 | 0 | 1 | 1 | 6 | insertion | shell | merge | 4.400 | 1.500 | 29 | 15 | 10 | merge | 10 | 1.500 |  |
| reverse | 31 | 16 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.500 | 1.500 | 0 | 36 | 24 | merge | 24 | 1.500 |  |
| reverse | 63 | 32 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.491 | 1.491 | 0 | 85 | 57 | merge | 57 | 1.491 |  |
| reverse | 48 | 32 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.488 | 1.488 | 0 | 61 | 41 | merge | 41 | 1.488 |  |
| reverse | 32 | 16 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.478 | 1.478 | 0 | 34 | 23 | merge | 23 | 1.478 |  |
| ascending_runs | 24 | 8 | 263 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.474 | 1.474 | 0 | 28 | 19 | merge | 19 | 1.474 |  |
| random | 47 | 32 | 263 | 143 | 0 | 1 | 1 | 10 | shell | shell | merge | 1.473 | 1.473 | 0 | 81 | 55 | merge | 55 | 1.473 | large_mixed |
| random_half | 48 | 32 | 197 | 31 | 0 | 1 | 1 | 3 | insertion | shell | merge | 6.106 | 1.468 | 218 | 69 | 47 | merge | 47 | 1.468 |  |
| few_unique_2 | 15 | 4 | 101 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 1.154 | 1.462 | 4 | 19 | 13 | merge | 13 | 1.462 |  |
| few_unique_2 | 15 | 4 | 263 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 1.231 | 1.462 | 3 | 19 | 13 | merge | 13 | 1.462 |  |
| few_unique_2 | 16 | 4 | 101 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 1.154 | 1.462 | 4 | 19 | 13 | merge | 13 | 1.462 |  |
| few_unique_2 | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 1.231 | 1.462 | 3 | 19 | 13 | merge | 13 | 1.462 |  |
| organ_pipe | 16 | 4 | 197 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 2.091 | 1.455 | 7 | 16 | 11 | merge | 11 | 1.455 |  |
| random_half | 16 | 4 | 101 | 85 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.182 | 1.455 | 19 | 16 | 11 | merge | 11 | 1.455 |  |
| ascending_runs | 16 | 4 | 131 | 51 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.727 | 1.455 | 25 | 16 | 11 | merge | 11 | 1.455 |  |
| organ_pipe | 64 | 32 | 131 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.448 | 1.448 | 0 | 84 | 58 | merge | 58 | 1.448 |  |
| organ_pipe | 32 | 16 | 197 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.440 | 1.440 | 0 | 36 | 25 | merge | 25 | 1.440 |  |
| reverse | 23 | 8 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.438 | 1.438 | 0 | 23 | 16 | merge | 16 | 1.438 |  |
| few_unique_2 | 15 | 4 | 131 | 127 | 0 | 1 | 1 | 13 | insertion | shell | merge | 1.071 | 1.429 | 5 | 20 | 14 | merge | 14 | 1.429 |  |
| few_unique_2 | 16 | 4 | 197 | 119 | 0 | 1 | 1 | 14 | insertion | shell | merge | 1.143 | 1.429 | 4 | 20 | 14 | merge | 14 | 1.429 |  |
| random | 15 | 4 | 131 | 127 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.417 | 1.417 | 0 | 17 | 12 | merge | 12 | 1.417 |  |
| reverse | 31 | 16 | 131 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.417 | 1.417 | 0 | 34 | 24 | merge | 24 | 1.417 |  |
| organ_pipe | 31 | 16 | 101 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.417 | 1.417 | 0 | 34 | 24 | merge | 24 | 1.417 |  |
| adversarial | 64 | 32 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.972 | 1.403 | 257 | 101 | 72 | merge | 72 | 1.403 |  |
| random | 8 | 2 | 101 | 145 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.800 | 1.400 | 2 | 7 | 5 | merge | 5 | 1.400 |  |
| stagger | 8 | 2 | 263 | 182 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.400 | 1.400 | 0 | 7 | 5 | merge | 5 | 1.400 |  |
| descending_blocks_4 | 15 | 4 | 131 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.100 | 1.400 | 3 | 14 | 10 | merge | 10 | 1.400 |  |
| random_half | 15 | 4 | 197 | 54 | 0 | 1 | 1 | 5 | insertion | shell | merge | 2.700 | 1.400 | 13 | 14 | 10 | merge | 10 | 1.400 |  |
| ascending_runs | 15 | 4 | 131 | 54 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.500 | 1.400 | 21 | 14 | 10 | merge | 10 | 1.400 |  |
| reverse | 16 | 4 | 131 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.900 | 1.400 | 35 | 14 | 10 | merge | 10 | 1.400 |  |
| descending_blocks_4 | 16 | 4 | 131 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.300 | 1.400 | 1 | 14 | 10 | merge | 10 | 1.400 |  |
| reverse | 32 | 16 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.400 | 1.400 | 0 | 35 | 25 | merge | 25 | 1.400 |  |
| organ_pipe | 64 | 32 | 197 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.397 | 1.397 | 0 | 81 | 58 | merge | 58 | 1.397 |  |
| random | 24 | 8 | 197 | 63 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.391 | 1.391 | 0 | 32 | 23 | merge | 23 | 1.391 |  |
| ascending_runs | 31 | 16 | 131 | 63 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.391 | 1.391 | 0 | 32 | 23 | merge | 23 | 1.391 |  |
| reverse | 48 | 32 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.390 | 1.390 | 0 | 57 | 41 | merge | 41 | 1.390 |  |
| random_half | 47 | 32 | 197 | 15 | 0 | 1 | 1 | 1 | insertion | shell | merge | 5.830 | 1.383 | 209 | 65 | 47 | merge | 47 | 1.383 |  |
| reverse | 23 | 8 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.375 | 1.375 | 0 | 22 | 16 | merge | 16 | 1.375 |  |
| random | 15 | 4 | 197 | 109 | 0 | 1 | 1 | 8 | insertion | shell | merge | 2.091 | 1.364 | 8 | 15 | 11 | merge | 11 | 1.364 |  |
| ascending_runs | 15 | 4 | 101 | 54 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.273 | 1.364 | 21 | 15 | 11 | merge | 11 | 1.364 |  |
| ascending_runs | 15 | 4 | 263 | 54 | 0 | 1 | 1 | 6 | insertion | shell | merge | 3.545 | 1.364 | 24 | 15 | 11 | merge | 11 | 1.364 |  |
| organ_pipe | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 2.000 | 1.364 | 7 | 15 | 11 | merge | 11 | 1.364 |  |
| organ_pipe | 16 | 4 | 263 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 2.000 | 1.364 | 7 | 15 | 11 | merge | 11 | 1.364 |  |
| descending_blocks_4 | 16 | 4 | 101 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.091 | 1.364 | 3 | 15 | 11 | merge | 11 | 1.364 |  |
| descending_runs | 16 | 4 | 101 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.182 | 1.364 | 2 | 15 | 11 | merge | 11 | 1.364 |  |
| descending_runs | 16 | 4 | 197 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 7.091 | 1.364 | 63 | 15 | 11 | merge | 11 | 1.364 |  |
| descending_runs | 16 | 4 | 263 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.091 | 1.364 | 3 | 15 | 11 | merge | 11 | 1.364 |  |
| adversarial | 16 | 4 | 197 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.000 | 1.364 | 7 | 15 | 11 | merge | 11 | 1.364 |  |
| adversarial | 63 | 32 | 101 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 5.097 | 1.361 | 269 | 98 | 72 | merge | 72 | 1.361 |  |
| organ_pipe | 64 | 32 | 263 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.361 | 1.361 | 0 | 83 | 61 | merge | 61 | 1.361 |  |
| reverse_first_half | 32 | 16 | 263 | 127 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.360 | 1.360 | 0 | 34 | 25 | merge | 25 | 1.360 |  |
| reverse | 23 | 8 | 263 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.353 | 1.353 | 0 | 23 | 17 | merge | 17 | 1.353 |  |
| organ_pipe | 32 | 16 | 131 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.346 | 1.346 | 0 | 35 | 26 | merge | 26 | 1.346 |  |
| descending_runs | 15 | 4 | 263 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.222 | 1.333 | 1 | 12 | 9 | merge | 9 | 1.333 |  |
| random_half | 23 | 8 | 101 | 63 | 0 | 1 | 1 | 2 | shell | shell | merge | 1.333 | 1.333 | 0 | 24 | 18 | merge | 18 | 1.333 |  |
| ascending_runs | 31 | 16 | 197 | 63 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.333 | 1.333 | 0 | 32 | 24 | merge | 24 | 1.333 |  |
| reverse | 32 | 16 | 101 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.333 | 1.333 | 0 | 32 | 24 | merge | 24 | 1.333 |  |
| organ_pipe | 63 | 32 | 197 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.333 | 1.333 | 0 | 76 | 57 | merge | 57 | 1.333 |  |
| organ_pipe | 64 | 32 | 101 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.333 | 1.333 | 0 | 76 | 57 | merge | 57 | 1.333 |  |
| random_tail | 64 | 32 | 131 | 47 | 0 | 1 | 1 | 3 | merge | shell | merge | 1.000 | 1.327 | 18 | 73 | 55 | merge | 55 | 1.327 |  |
| organ_pipe | 31 | 16 | 131 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.320 | 1.320 | 0 | 33 | 25 | merge | 25 | 1.320 |  |
| adversarial | 24 | 8 | 263 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 2.409 | 1.318 | 24 | 29 | 22 | merge | 22 | 1.318 |  |
| random_half | 23 | 8 | 197 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.316 | 1.316 | 0 | 25 | 19 | merge | 19 | 1.316 |  |
| organ_pipe | 63 | 32 | 101 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.316 | 1.316 | 0 | 75 | 57 | merge | 57 | 1.316 |  |
| random_half | 48 | 32 | 101 | 63 | 0 | 1 | 1 | 6 | merge | shell | merge | 1.000 | 1.311 | 14 | 59 | 45 | merge | 45 | 1.311 |  |
| organ_pipe | 31 | 16 | 197 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.308 | 1.308 | 0 | 34 | 26 | merge | 26 | 1.308 |  |
| descending_blocks_4 | 15 | 4 | 101 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.100 | 1.300 | 2 | 13 | 10 | merge | 10 | 1.300 |  |
| descending_blocks_4 | 15 | 4 | 197 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.100 | 1.300 | 2 | 13 | 10 | merge | 10 | 1.300 |  |
| random_half | 15 | 4 | 101 | 72 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.000 | 1.300 | 17 | 13 | 10 | merge | 10 | 1.300 |  |
| random_half | 15 | 4 | 131 | 54 | 0 | 1 | 1 | 6 | insertion | shell | merge | 2.400 | 1.300 | 11 | 13 | 10 | merge | 10 | 1.300 |  |
| descending_runs | 15 | 4 | 101 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.100 | 1.300 | 2 | 13 | 10 | merge | 10 | 1.300 |  |
| descending_runs | 15 | 4 | 197 | 200 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.100 | 1.300 | 2 | 13 | 10 | merge | 10 | 1.300 |  |
| adversarial | 15 | 4 | 101 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.800 | 1.300 | 5 | 13 | 10 | merge | 10 | 1.300 |  |
| adversarial | 15 | 4 | 131 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.800 | 1.300 | 5 | 13 | 10 | merge | 10 | 1.300 |  |
| adversarial | 15 | 4 | 197 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.700 | 1.300 | 4 | 13 | 10 | merge | 10 | 1.300 |  |
| adversarial | 15 | 4 | 263 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.700 | 1.300 | 4 | 13 | 10 | merge | 10 | 1.300 |  |
| adversarial | 47 | 32 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.880 | 1.300 | 129 | 65 | 50 | merge | 50 | 1.300 |  |
| adversarial | 31 | 16 | 101 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.407 | 1.296 | 57 | 35 | 27 | merge | 27 | 1.296 |  |
| adversarial | 32 | 16 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.677 | 1.290 | 105 | 40 | 31 | merge | 31 | 1.290 |  |
| adversarial | 24 | 8 | 101 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 2.381 | 1.286 | 23 | 27 | 21 | merge | 21 | 1.286 |  |
| adversarial | 31 | 16 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.000 | 1.286 | 48 | 36 | 28 | merge | 28 | 1.286 |  |
| adversarial | 48 | 32 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.940 | 1.280 | 133 | 64 | 50 | merge | 50 | 1.280 |  |
| reverse | 23 | 8 | 197 | 255 | 0 | 0 | 1 | 0 | shell | shell | merge | 1.278 | 1.278 | 0 | 23 | 18 | merge | 18 | 1.278 |  |
| random | 15 | 4 | 101 | 145 | 0 | 1 | 1 | 9 | insertion | shell | merge | 2.545 | 1.273 | 14 | 14 | 11 | merge | 11 | 1.273 |  |
| few_unique_8 | 15 | 4 | 263 | 163 | 0 | 1 | 1 | 10 | insertion | shell | merge | 1.909 | 1.273 | 7 | 14 | 11 | merge | 11 | 1.273 |  |
| reverse | 16 | 4 | 263 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.727 | 1.273 | 38 | 14 | 11 | merge | 11 | 1.273 |  |
| random_half | 16 | 4 | 197 | 68 | 0 | 1 | 1 | 5 | insertion | shell | merge | 3.000 | 1.273 | 19 | 14 | 11 | merge | 11 | 1.273 |  |
| descending_runs | 16 | 4 | 131 | 204 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.182 | 1.273 | 1 | 14 | 11 | merge | 11 | 1.273 |  |
| adversarial | 16 | 4 | 101 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.909 | 1.273 | 7 | 14 | 11 | merge | 11 | 1.273 |  |
| adversarial | 16 | 4 | 263 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.091 | 1.273 | 9 | 14 | 11 | merge | 11 | 1.273 |  |
| adversarial | 24 | 8 | 131 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 2.409 | 1.273 | 25 | 28 | 22 | merge | 22 | 1.273 |  |
| random_tail | 63 | 32 | 131 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.055 | 1.273 | 12 | 70 | 55 | merge | 55 | 1.273 |  |
| adversarial | 63 | 32 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 5.229 | 1.271 | 277 | 89 | 70 | merge | 70 | 1.271 |  |
| organ_pipe | 31 | 16 | 263 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.269 | 1.269 | 0 | 33 | 26 | merge | 26 | 1.269 |  |
| organ_pipe | 32 | 16 | 101 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.269 | 1.269 | 0 | 33 | 26 | merge | 26 | 1.269 |  |
| adversarial | 31 | 16 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.833 | 1.267 | 47 | 38 | 30 | merge | 30 | 1.267 |  |
| adversarial | 64 | 32 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.532 | 1.266 | 258 | 100 | 79 | merge | 79 | 1.266 |  |
| adversarial | 47 | 32 | 101 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.959 | 1.265 | 132 | 62 | 49 | merge | 49 | 1.265 |  |
| adversarial | 47 | 32 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.347 | 1.265 | 151 | 62 | 49 | merge | 49 | 1.265 |  |
| adversarial | 23 | 8 | 263 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.263 | 1.263 | 0 | 24 | 19 | merge | 19 | 1.263 |  |
| random_half | 24 | 8 | 131 | 63 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.263 | 1.263 | 0 | 24 | 19 | merge | 19 | 1.263 |  |
| random_half | 24 | 8 | 263 | 47 | 0 | 1 | 1 | 6 | shell | shell | merge | 1.263 | 1.263 | 0 | 24 | 19 | merge | 19 | 1.263 |  |
| adversarial | 64 | 32 | 101 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.904 | 1.260 | 266 | 92 | 73 | merge | 73 | 1.260 |  |
| adversarial | 48 | 32 | 101 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.582 | 1.255 | 128 | 69 | 55 | merge | 55 | 1.255 |  |
| few_unique_8 | 64 | 32 | 263 | 127 | 0 | 1 | 1 | 15 | shell | shell | merge | 1.254 | 1.254 | 0 | 89 | 71 | merge | 71 | 1.254 | large_mixed |
| reverse | 7 | 2 | 131 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 2.500 | 1.250 | 5 | 5 | 4 | merge | 4 | 1.250 |  |
| reverse | 7 | 2 | 263 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 2.500 | 1.250 | 5 | 5 | 4 | merge | 4 | 1.250 |  |
| stagger | 7 | 2 | 263 | 170 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.500 | 1.250 | 1 | 5 | 4 | merge | 4 | 1.250 |  |
| descending_blocks_4 | 7 | 2 | 101 | 212 | 0 | 1 | 1 | 2 | insertion | shell | merge | 41.000 | 1.250 | 159 | 5 | 4 | merge | 4 | 1.250 |  |
| descending_blocks_4 | 7 | 2 | 263 | 212 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.250 | 1.250 | 0 | 5 | 4 | merge | 4 | 1.250 |  |
| alternating_high_low | 7 | 2 | 101 | 127 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.250 | 1.250 | 0 | 5 | 4 | merge | 4 | 1.250 |  |
| alternating_high_low | 7 | 2 | 197 | 127 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.250 | 1.250 | 0 | 5 | 4 | merge | 4 | 1.250 |  |
| alternating_high_low | 7 | 2 | 263 | 127 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.250 | 1.250 | 0 | 5 | 4 | merge | 4 | 1.250 |  |
| few_unique_8 | 8 | 2 | 197 | 145 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.250 | 1.250 | 0 | 5 | 4 | merge | 4 | 1.250 |  |
| adversarial | 16 | 4 | 131 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.917 | 1.250 | 8 | 15 | 12 | merge | 12 | 1.250 |  |
| adversarial | 23 | 8 | 131 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.250 | 1.250 | 0 | 25 | 20 | merge | 20 | 1.250 |  |
| organ_pipe | 32 | 16 | 263 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.250 | 1.250 | 0 | 35 | 28 | merge | 28 | 1.250 |  |
| adversarial | 32 | 16 | 101 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.062 | 1.250 | 58 | 40 | 32 | merge | 32 | 1.250 |  |
| adversarial | 32 | 16 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.062 | 1.250 | 58 | 40 | 32 | merge | 32 | 1.250 |  |
| adversarial | 32 | 16 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.062 | 1.250 | 58 | 40 | 32 | merge | 32 | 1.250 |  |
| organ_pipe | 47 | 32 | 131 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.244 | 1.244 | 0 | 51 | 41 | merge | 41 | 1.244 |  |
| adversarial | 23 | 8 | 101 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.238 | 1.238 | 0 | 26 | 21 | merge | 21 | 1.238 |  |
| adversarial | 23 | 8 | 197 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.238 | 1.238 | 0 | 26 | 21 | merge | 21 | 1.238 |  |
| organ_pipe | 48 | 32 | 197 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.238 | 1.238 | 0 | 52 | 42 | merge | 42 | 1.238 |  |
| adversarial | 63 | 32 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.766 | 1.234 | 272 | 95 | 77 | merge | 77 | 1.234 |  |
| reverse | 15 | 4 | 197 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.778 | 1.222 | 32 | 11 | 9 | merge | 9 | 1.222 |  |
| descending_blocks_4 | 47 | 32 | 131 | 191 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.211 | 1.211 | 0 | 46 | 38 | merge | 38 | 1.211 | large_mixed |
| organ_pipe | 47 | 32 | 101 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.205 | 1.205 | 0 | 53 | 44 | merge | 44 | 1.205 |  |
| alternating_high_low | 8 | 2 | 101 | 109 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.200 | 1.200 | 0 | 6 | 5 | merge | 5 | 1.200 |  |
| alternating_high_low | 8 | 2 | 131 | 109 | 0 | 1 | 1 | 6 | insertion | shell | merge | 1.200 | 1.200 | 0 | 6 | 5 | merge | 5 | 1.200 |  |
| reverse_second_half | 15 | 4 | 131 | 127 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.500 | 1.200 | 3 | 12 | 10 | merge | 10 | 1.200 |  |
| reverse_second_half | 16 | 4 | 263 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.400 | 1.200 | 2 | 12 | 10 | merge | 10 | 1.200 |  |
| few_unique_8 | 64 | 32 | 131 | 143 | 0 | 1 | 1 | 14 | shell | shell | merge | 1.200 | 1.200 | 0 | 90 | 75 | merge | 75 | 1.200 | large_mixed |
| few_unique_8 | 63 | 32 | 131 | 127 | 0 | 1 | 1 | 13 | shell | shell | merge | 1.197 | 1.197 | 0 | 91 | 76 | merge | 76 | 1.197 | large_mixed |
| few_unique_8 | 63 | 32 | 263 | 143 | 0 | 1 | 1 | 14 | shell | shell | merge | 1.197 | 1.197 | 0 | 85 | 71 | merge | 71 | 1.197 | large_mixed |
| organ_pipe | 47 | 32 | 263 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.195 | 1.195 | 0 | 49 | 41 | merge | 41 | 1.195 |  |
| organ_pipe | 63 | 32 | 263 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.194 | 1.194 | 0 | 74 | 62 | merge | 62 | 1.194 |  |
| few_unique_8 | 63 | 32 | 101 | 175 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.192 | 1.192 | 0 | 87 | 73 | merge | 73 | 1.192 | large_mixed |
| organ_pipe | 63 | 32 | 131 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.190 | 1.190 | 0 | 75 | 63 | merge | 63 | 1.190 |  |
| adversarial | 48 | 32 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.981 | 1.185 | 151 | 64 | 54 | merge | 54 | 1.185 |  |
| descending_blocks_4 | 47 | 32 | 197 | 191 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.184 | 1.184 | 0 | 45 | 38 | merge | 38 | 1.184 | large_mixed |
| few_unique_8 | 15 | 4 | 131 | 163 | 0 | 1 | 1 | 9 | insertion | shell | merge | 2.364 | 1.182 | 13 | 13 | 11 | merge | 11 | 1.182 |  |
| organ_pipe | 15 | 4 | 197 | 109 | 1 | 1 | 1 | 1 | insertion | shell | merge | 1.909 | 1.182 | 8 | 13 | 11 | merge | 11 | 1.182 |  |
| reverse | 16 | 4 | 101 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.364 | 1.182 | 35 | 13 | 11 | merge | 11 | 1.182 |  |
| reverse | 16 | 4 | 197 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.364 | 1.182 | 35 | 13 | 11 | merge | 11 | 1.182 |  |
| adversarial | 24 | 8 | 197 | 15 | 0 | 1 | 1 | 2 | insertion | shell | merge | 2.455 | 1.182 | 28 | 26 | 22 | merge | 22 | 1.182 |  |
| adversarial | 64 | 32 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.949 | 1.177 | 298 | 93 | 79 | merge | 79 | 1.177 |  |
| adversarial | 63 | 32 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 4.623 | 1.169 | 266 | 90 | 77 | merge | 77 | 1.169 |  |
| random | 15 | 4 | 263 | 109 | 0 | 1 | 1 | 7 | insertion | shell | merge | 2.083 | 1.167 | 11 | 14 | 12 | merge | 12 | 1.167 |  |
| few_unique_8 | 15 | 4 | 197 | 145 | 0 | 1 | 1 | 10 | insertion | shell | merge | 1.750 | 1.167 | 7 | 14 | 12 | merge | 12 | 1.167 |  |
| few_unique_8 | 16 | 4 | 101 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.917 | 1.167 | 9 | 14 | 12 | merge | 12 | 1.167 |  |
| few_unique_8 | 16 | 4 | 131 | 170 | 0 | 1 | 1 | 10 | insertion | shell | merge | 2.583 | 1.167 | 17 | 14 | 12 | merge | 12 | 1.167 |  |
| few_unique_8 | 16 | 4 | 263 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.750 | 1.167 | 7 | 14 | 12 | merge | 12 | 1.167 |  |
| organ_pipe | 16 | 4 | 101 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.833 | 1.167 | 8 | 14 | 12 | merge | 12 | 1.167 |  |
| stagger | 16 | 4 | 101 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 2.250 | 1.167 | 13 | 14 | 12 | merge | 12 | 1.167 |  |
| stagger | 16 | 4 | 131 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 2.000 | 1.167 | 10 | 14 | 12 | merge | 12 | 1.167 |  |
| stagger | 16 | 4 | 197 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 2.167 | 1.167 | 12 | 14 | 12 | merge | 12 | 1.167 |  |
| adversarial | 31 | 16 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.067 | 1.167 | 57 | 35 | 30 | merge | 30 | 1.167 |  |
| random | 16 | 4 | 101 | 136 | 0 | 1 | 1 | 10 | insertion | shell | merge | 2.000 | 1.154 | 11 | 15 | 13 | merge | 13 | 1.154 |  |
| organ_pipe | 48 | 32 | 101 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.143 | 1.143 | 0 | 48 | 42 | merge | 42 | 1.143 |  |
| stagger | 48 | 32 | 131 | 143 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.143 | 1.143 | 0 | 56 | 49 | merge | 49 | 1.143 | large_mixed |
| random_tail | 64 | 32 | 197 | 31 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.054 | 1.143 | 5 | 64 | 56 | merge | 56 | 1.143 |  |
| organ_pipe | 48 | 32 | 131 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.133 | 1.133 | 0 | 51 | 45 | merge | 45 | 1.133 |  |
| adversarial | 47 | 32 | 263 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 3.815 | 1.130 | 145 | 61 | 54 | merge | 54 | 1.130 |  |
| few_unique_8 | 64 | 32 | 101 | 111 | 0 | 1 | 1 | 14 | shell | shell | merge | 1.128 | 1.128 | 0 | 88 | 78 | merge | 78 | 1.128 | large_mixed |
| organ_pipe | 47 | 32 | 197 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.114 | 1.114 | 0 | 49 | 44 | merge | 44 | 1.114 |  |
| few_unique_8 | 64 | 32 | 197 | 111 | 0 | 1 | 1 | 14 | shell | shell | merge | 1.113 | 1.113 | 0 | 89 | 80 | merge | 80 | 1.113 | large_mixed |
| random_half | 23 | 8 | 263 | 63 | 0 | 1 | 1 | 4 | shell | shell | merge | 1.105 | 1.105 | 0 | 21 | 19 | merge | 19 | 1.105 |  |
| sawtooth | 31 | 16 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.793 | 1.103 | 49 | 32 | 29 | merge | 29 | 1.103 |  |
| few_unique_8 | 63 | 32 | 197 | 175 | 0 | 1 | 1 | 7 | shell | shell | merge | 1.101 | 1.101 | 0 | 87 | 79 | merge | 79 | 1.101 | large_mixed |
| reverse | 15 | 4 | 101 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.500 | 1.100 | 34 | 11 | 10 | merge | 10 | 1.100 |  |
| reverse | 15 | 4 | 131 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.600 | 1.100 | 35 | 11 | 10 | merge | 10 | 1.100 |  |
| reverse | 15 | 4 | 263 | 255 | 0 | 0 | 1 | 0 | insertion | shell | merge | 4.600 | 1.100 | 35 | 11 | 10 | merge | 10 | 1.100 |  |
| reverse_second_half | 15 | 4 | 101 | 127 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.400 | 1.100 | 3 | 11 | 10 | merge | 10 | 1.100 |  |
| reverse_first_half | 16 | 4 | 101 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.400 | 1.100 | 3 | 11 | 10 | merge | 10 | 1.100 |  |
| reverse_first_half | 16 | 4 | 197 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.300 | 1.100 | 2 | 11 | 10 | merge | 10 | 1.100 |  |
| reverse_second_half | 16 | 4 | 101 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.300 | 1.100 | 2 | 11 | 10 | merge | 10 | 1.100 |  |
| organ_pipe | 15 | 4 | 101 | 109 | 1 | 1 | 1 | 1 | insertion | shell | merge | 16.091 | 1.091 | 165 | 12 | 11 | merge | 11 | 1.091 |  |
| organ_pipe | 15 | 4 | 263 | 109 | 1 | 1 | 1 | 1 | insertion | shell | merge | 1.909 | 1.091 | 9 | 12 | 11 | merge | 11 | 1.091 |  |
| sawtooth | 15 | 4 | 263 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.455 | 1.091 | 4 | 12 | 11 | merge | 11 | 1.091 |  |
| reverse_second_half | 15 | 4 | 263 | 127 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.273 | 1.091 | 2 | 12 | 11 | merge | 11 | 1.091 |  |
| reverse_second_half | 16 | 4 | 197 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.273 | 1.091 | 2 | 12 | 11 | merge | 11 | 1.091 |  |
| organ_pipe | 15 | 4 | 131 | 109 | 1 | 1 | 1 | 1 | insertion | shell | merge | 1.750 | 1.083 | 8 | 13 | 12 | merge | 12 | 1.083 |  |
| stagger | 15 | 4 | 131 | 145 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.667 | 1.083 | 7 | 13 | 12 | merge | 12 | 1.083 |  |
| few_unique_8 | 15 | 4 | 101 | 145 | 0 | 1 | 1 | 10 | insertion | shell | merge | 1.769 | 1.077 | 9 | 14 | 13 | merge | 13 | 1.077 |  |
| few_unique_8 | 16 | 4 | 197 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.923 | 1.077 | 11 | 14 | 13 | merge | 13 | 1.077 |  |
| stagger | 16 | 4 | 263 | 153 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.846 | 1.077 | 10 | 14 | 13 | merge | 13 | 1.077 |  |
| stagger | 48 | 32 | 263 | 143 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.075 | 1.075 | 0 | 57 | 53 | merge | 53 | 1.075 | large_mixed |
| organ_pipe | 48 | 32 | 263 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.067 | 1.067 | 0 | 48 | 45 | merge | 45 | 1.067 |  |
| sawtooth | 32 | 16 | 197 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.774 | 1.065 | 53 | 33 | 31 | merge | 31 | 1.065 |  |
| random_half | 23 | 8 | 131 | 63 | 0 | 1 | 1 | 8 | shell | shell | merge | 1.050 | 1.050 | 0 | 21 | 20 | merge | 20 | 1.050 |  |
| organ_pipe | 24 | 8 | 101 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.050 | 1.050 | 0 | 21 | 20 | merge | 20 | 1.050 |  |
| organ_pipe | 24 | 8 | 131 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.050 | 1.050 | 0 | 21 | 20 | merge | 20 | 1.050 |  |
| organ_pipe | 24 | 8 | 197 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.050 | 1.050 | 0 | 21 | 20 | merge | 20 | 1.050 |  |
| reverse_second_half | 47 | 32 | 197 | 127 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.050 | 1.050 | 0 | 42 | 40 | merge | 40 | 1.050 |  |
| reverse_second_half | 48 | 32 | 131 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.050 | 1.050 | 0 | 42 | 40 | merge | 40 | 1.050 |  |
| reverse_second_half | 47 | 32 | 101 | 127 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.025 | 1.025 | 0 | 41 | 40 | merge | 40 | 1.025 |  |
| stagger | 48 | 32 | 197 | 143 | 0 | 1 | 1 | 11 | shell | shell | merge | 1.020 | 1.020 | 0 | 51 | 50 | merge | 50 | 1.020 | large_mixed |
| few_unique_8 | 7 | 2 | 197 | 127 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.250 | 1.000 | 1 | 4 | 4 | merge | 4 | 1.000 |  |
| random_half | 7 | 2 | 101 | 85 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| random_half | 7 | 2 | 131 | 85 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| random_half | 7 | 2 | 197 | 85 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| random_half | 7 | 2 | 263 | 42 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.250 | 1.000 | 1 | 4 | 4 | merge | 4 | 1.000 |  |
| ascending_runs | 7 | 2 | 101 | 42 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| ascending_runs | 7 | 2 | 131 | 42 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| ascending_runs | 7 | 2 | 197 | 42 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| ascending_runs | 7 | 2 | 263 | 42 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.500 | 1.000 | 2 | 4 | 4 | merge | 4 | 1.000 |  |
| random | 8 | 2 | 131 | 109 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.250 | 1.000 | 1 | 4 | 4 | merge | 4 | 1.000 |  |
| random | 8 | 2 | 197 | 109 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.250 | 1.000 | 1 | 4 | 4 | merge | 4 | 1.000 |  |
| few_unique_8 | 8 | 2 | 101 | 145 | 0 | 1 | 1 | 5 | insertion | shell | merge | 1.250 | 1.000 | 1 | 4 | 4 | merge | 4 | 1.000 |  |
| few_unique_8 | 8 | 2 | 131 | 182 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.600 | 1.000 | 3 | 5 | 5 | merge | 5 | 1.000 |  |
| random_half | 8 | 2 | 101 | 109 | 0 | 1 | 1 | 2 | insertion | shell | merge | 2.000 | 1.000 | 5 | 5 | 5 | merge | 5 | 1.000 |  |
| random_half | 8 | 2 | 131 | 72 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.600 | 1.000 | 3 | 5 | 5 | merge | 5 | 1.000 |  |
| random_half | 8 | 2 | 197 | 72 | 0 | 1 | 1 | 3 | insertion | shell | merge | 2.250 | 1.000 | 5 | 4 | 4 | merge | 4 | 1.000 |  |
| random_half | 8 | 2 | 263 | 72 | 0 | 1 | 1 | 2 | insertion | shell | merge | 1.800 | 1.000 | 4 | 5 | 5 | merge | 5 | 1.000 |  |
| stagger | 15 | 4 | 101 | 145 | 0 | 1 | 1 | 11 | insertion | shell | merge | 1.615 | 1.000 | 8 | 13 | 13 | merge | 13 | 1.000 |  |
| sawtooth | 15 | 4 | 101 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.500 | 1.000 | 6 | 12 | 12 | merge | 12 | 1.000 |  |
| sawtooth | 15 | 4 | 131 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.417 | 1.000 | 5 | 12 | 12 | merge | 12 | 1.000 |  |
| sawtooth | 15 | 4 | 197 | 36 | 0 | 1 | 1 | 3 | insertion | shell | merge | 1.545 | 1.000 | 6 | 11 | 11 | merge | 11 | 1.000 |  |
| reverse_second_half | 15 | 4 | 197 | 127 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.364 | 1.000 | 4 | 11 | 11 | merge | 11 | 1.000 |  |
| sawtooth | 16 | 4 | 101 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.692 | 1.000 | 9 | 13 | 13 | merge | 13 | 1.000 |  |
| sawtooth | 16 | 4 | 131 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.615 | 1.000 | 8 | 13 | 13 | merge | 13 | 1.000 |  |
| sawtooth | 16 | 4 | 197 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.833 | 1.000 | 10 | 12 | 12 | merge | 12 | 1.000 |  |
| sawtooth | 16 | 4 | 263 | 34 | 0 | 1 | 1 | 4 | insertion | shell | merge | 1.692 | 1.000 | 9 | 13 | 13 | merge | 13 | 1.000 |  |
| reverse_first_half | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.273 | 1.000 | 3 | 11 | 11 | merge | 11 | 1.000 |  |
| reverse_second_half | 16 | 4 | 131 | 119 | 0 | 1 | 1 | 1 | insertion | shell | merge | 1.182 | 1.000 | 2 | 11 | 11 | merge | 11 | 1.000 |  |
| organ_pipe | 23 | 8 | 197 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.000 | 1.000 | 0 | 18 | 18 | merge | 18 | 1.000 |  |
| organ_pipe | 23 | 8 | 263 | 111 | 1 | 1 | 1 | 1 | shell | shell | merge | 1.000 | 1.000 | 0 | 20 | 20 | merge | 20 | 1.000 |  |
| sawtooth | 23 | 8 | 131 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.000 | 1.000 | 0 | 21 | 21 | merge | 21 | 1.000 |  |
| sawtooth | 23 | 8 | 197 | 47 | 0 | 1 | 1 | 5 | shell | shell | merge | 1.000 | 1.000 | 0 | 21 | 21 | merge | 21 | 1.000 |  |
| organ_pipe | 24 | 8 | 263 | 111 | 0 | 1 | 1 | 1 | shell | shell | merge | 1.000 | 1.000 | 0 | 19 | 19 | merge | 19 | 1.000 |  |
| sawtooth | 32 | 16 | 131 | 31 | 0 | 1 | 1 | 4 | insertion | shell | merge | 2.848 | 1.000 | 61 | 33 | 33 | merge | 33 | 1.000 |  |
