#ifndef LOXSORT_GENERATED_LOX_PROFILE_GENERIC_GENERATED_H
#define LOXSORT_GENERATED_LOX_PROFILE_GENERIC_GENERATED_H

#include "loxsort/loxsort.h"

const lox_profile_t lox_profile_generic = {
    .format_version = LOX_PROFILE_FORMAT_VERSION,
    .algorithm_mask =
        LOX_ALGORITHM_MASK_INSERTION |
#if LOXSORT_ENABLE_SHELL
        LOX_ALGORITHM_MASK_SHELL |
#endif
#if LOXSORT_ENABLE_INTRO
        LOX_ALGORITHM_MASK_INTRO |
#endif
#if LOXSORT_ENABLE_MERGE
        LOX_ALGORITHM_MASK_MERGE |
#endif
#if LOXSORT_ENABLE_CYCLE
        LOX_ALGORITHM_MASK_CYCLE |
#endif
        0u,
    .small_count_cutoff = 16u,
    .duplicate_intro_min_count = 16u,
    .duplicate_intro_max_count = 64u,
    .stable_insertion_cutoff = 32u,
    .near_sorted_insertion_max_count = 64u,
    .cycle_min_count = 64u,
    .shell_max_count_elem_small = 256u,
    .shell_max_count_elem_medium = 512u,
    .shell_max_count_elem_large = 1024u,
    .merge_min_count_elem_small = 0u,
    .merge_min_count_elem_medium = 0u,
    .merge_min_count_elem_large = 0u,
    .merge_min_count = 96u,
    .merge_min_data_bytes = 4u,
    .chaotic_merge_min_count = 96u,
    .chaotic_merge_min_data_bytes = 6u,
    .chaotic_merge_data_bytes_max = 96u,
    .chaotic_merge_direction_min = 4u,
    .chaotic_merge_disorder_min = 16u,
    .merge_large_mixed_min_count = 16u,
    .merge_large_mixed_min_data_bytes = 32u,
    .merge_large_mixed_direction_min = 4u,
    .merge_large_mixed_direction_max = 16u,
    .merge_large_mixed_disorder_min = 0u,
    .duplicate_equal_min = 0u,
    .duplicate_disorder_min = 0u,
    .merge_duplicate_equal_min = 0u,
    .merge_duplicate_disorder_min = 0u,
    .sample_pairs = 16u,
    .near_sorted_disorder_max = 32u,
    .near_sorted_direction_changes_max = 4u,
    .cycle_equal_score_max = 0u,
    .reserved0 = 0u
};

#endif
