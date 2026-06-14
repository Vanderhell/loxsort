# LoxSort profile report

## Selected profile
- format_version: 3
- algorithm_mask: 31
- small_count_cutoff: 8
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
- sample_pairs: 16
- near_sorted_disorder_max: 12
- near_sorted_direction_changes_max: 4
- duplicate_equal_min: 6
- duplicate_disorder_min: 96
- cycle_equal_score_max: 32

## Dataset split
- training scenarios: 1564
- validation scenarios: 1564
- test scenarios: 1564
- training seeds: [1, 7, 13, 29]
- validation seeds: [101, 131, 197, 263]
- test seeds: [401, 433, 467, 499]
- training digest: 9bf94f889b182229ac2c668c93d06afc77d931185f99ce881e023cd7e5766d56
- validation digest: 6076f2e1f04231af9482b22cddd444c60d27f8b6f9a2f9b99737acda2bb22bf4
- test digest: b7906039b86234694c4ef7557ca18c43772a4728c22565be3475a99b29420b1a

## Regression guard
- training improved: 156
- training worsened: 143
- training unchanged: 1265
- validation improved: 159
- validation worsened: 147
- validation unchanged: 1258
- test improved: 153
- test worsened: 145
- test unchanged: 1266

## Split regret
### training
- reported scenarios: 1564
- worst cases:
  - ('descending_runs', 4, 1, 7, 3, 255, 0, 0, 0, 1, 0): regret=4.000
  - ('few_unique_2', 23, 8, 29, 16, 127, 0, 0, 1, 1, 10): regret=3.583
  - ('few_unique_2', 24, 8, 7, 16, 111, 0, 0, 1, 1, 9): regret=3.583
  - ('few_unique_2', 24, 8, 13, 16, 111, 0, 0, 1, 1, 9): regret=3.583
  - ('few_unique_2', 24, 8, 29, 16, 111, 0, 0, 1, 1, 9): regret=3.500

- p50_regret: 1.083
- p95_regret: 2.044
- max_regret: 4.000

### validation
- reported scenarios: 1564
- worst cases:
  - ('few_unique_2', 23, 8, 263, 16, 127, 0, 0, 1, 1, 10): regret=3.667
  - ('few_unique_2', 23, 8, 101, 16, 127, 0, 0, 1, 1, 10): regret=3.583
  - ('few_unique_2', 23, 8, 197, 16, 127, 0, 0, 1, 1, 10): regret=3.583
  - ('few_unique_2', 23, 8, 131, 16, 127, 0, 0, 1, 1, 10): regret=3.500
  - ('few_unique_2', 24, 8, 197, 16, 111, 0, 0, 1, 1, 9): regret=3.462

- p50_regret: 1.053
- p95_regret: 2.000
- max_regret: 3.667

### test
- reported scenarios: 1564
- worst cases:
  - ('shuffled_runs', 4, 1, 433, 3, 255, 0, 0, 0, 1, 0): regret=4.000
  - ('few_unique_2', 23, 8, 433, 16, 127, 0, 0, 1, 1, 10): regret=3.667
  - ('few_unique_2', 23, 8, 499, 16, 127, 0, 0, 1, 1, 10): regret=3.667
  - ('few_unique_2', 24, 8, 467, 16, 111, 0, 0, 1, 1, 9): regret=3.667
  - ('reverse', 8, 2, 433, 7, 255, 0, 0, 0, 1, 0): regret=3.500

- p50_regret: 1.083
- p95_regret: 2.031
- max_regret: 4.000

## Worst validation cases
- pattern: few_unique_2
  count: 23
  element_size: 8
  features: dataset_seed=263, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=10
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.667
- pattern: few_unique_2
  count: 23
  element_size: 8
  features: dataset_seed=101, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=10
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.583
- pattern: few_unique_2
  count: 23
  element_size: 8
  features: dataset_seed=197, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=10
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.583
- pattern: few_unique_2
  count: 23
  element_size: 8
  features: dataset_seed=131, sampled_pair_count=16, disorder_score=127, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=10
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.500
- pattern: few_unique_2
  count: 24
  element_size: 8
  features: dataset_seed=197, sampled_pair_count=16, disorder_score=111, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=9
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.462
- pattern: few_unique_2
  count: 24
  element_size: 8
  features: dataset_seed=101, sampled_pair_count=16, disorder_score=111, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=9
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.385
- pattern: few_unique_2
  count: 24
  element_size: 8
  features: dataset_seed=131, sampled_pair_count=16, disorder_score=111, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=9
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.385
- pattern: few_unique_2
  count: 24
  element_size: 8
  features: dataset_seed=263, sampled_pair_count=16, disorder_score=111, equal_score=0, equal_pair_count=0, has_less=1, has_greater=1, direction_changes=9
  selected_algorithm: shell
  best_algorithm: intro
  regret: 3.385
- pattern: reverse
  count: 8
  element_size: 2
  features: dataset_seed=101, sampled_pair_count=7, disorder_score=255, equal_score=0, equal_pair_count=0, has_less=0, has_greater=1, direction_changes=0
  selected_algorithm: insertion
  best_algorithm: merge
  regret: 2.800
- pattern: reverse
  count: 8
  element_size: 2
  features: dataset_seed=263, sampled_pair_count=7, disorder_score=255, equal_score=0, equal_pair_count=0, has_less=0, has_greater=1, direction_changes=0
  selected_algorithm: insertion
  best_algorithm: merge
  regret: 2.800
