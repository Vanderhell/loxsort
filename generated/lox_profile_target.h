#ifndef LOXSORT_GENERATED_PROFILE_H
#define LOXSORT_GENERATED_PROFILE_H
#include "loxsort/loxsort.h"

/* input_sha256: 507e2493476872c239c0b57df5acc4db7e93bbeaeec6e193fc97dfb0bf8e5ba8 */
static const lox_profile_t lox_profile_target = {
    .format_version = 7u,
    .algorithm_mask = 31u,
    .small_count_cutoff = 4u,
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
    .merge_min_count = 20u,
    .merge_min_data_bytes = 96u,
    .chaotic_merge_min_count = 23u,
    .chaotic_merge_min_data_bytes = 128u,
    .chaotic_merge_data_bytes_max = 512u,
    .chaotic_merge_direction_min = 4u,
    .chaotic_merge_disorder_min = 16u,
    .merge_large_mixed_min_count = 31u,
    .merge_large_mixed_min_data_bytes = 96u,
    .merge_large_mixed_direction_min = 2u,
    .merge_large_mixed_direction_max = 6u,
    .merge_large_mixed_disorder_min = 16u,
    .duplicate_equal_min = 6u,
    .duplicate_disorder_min = 96u,
    .merge_duplicate_equal_min = 0u,
    .merge_duplicate_disorder_min = 0u,
    .sample_pairs = 16u,
    .near_sorted_disorder_max = 12u,
    .near_sorted_direction_changes_max = 4u,
    .cycle_equal_score_max = 32u,
    .reserved0 = 0u
};

#endif
