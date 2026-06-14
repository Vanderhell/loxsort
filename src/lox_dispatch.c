#include "lox_internal.h"

static void lox_init_result(lox_sort_result_t *result)
{
    if (result != NULL) {
        result->algorithm = LOX_ALGORITHM_NONE;
        result->reason = LOX_REASON_TRIVIAL_INPUT;
        result->profile_fallback_used = 0u;
        result->sampled_pair_count = 0u;
        result->disorder_score = 0u;
        result->equal_score = 0u;
        result->equal_pair_count = 0u;
        result->direction_changes = 0u;
        result->comparison_sign_mask = 0u;
    }
}

static int lox_validate_options(const lox_sort_options_t *options)
{
    if (options == NULL) {
        return 1;
    }
    if ((options->flags & (uint32_t)~LOXSORT_SORT_FLAG_MASK) != 0u) {
        return 0;
    }
    if (options->scratch == NULL && options->scratch_size != 0u) {
        return 0;
    }
    return 1;
}

static int lox_algorithm_enabled_by_mask(uint16_t mask, uint16_t bit)
{
    return (mask & bit) != 0u;
}

static const lox_algorithm_entry_t *lox_pick_entry(lox_algorithm_t algorithm)
{
    return lox_registry_find(algorithm);
}

static lox_status_t lox_verify_if_requested(
    const void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    uint32_t flags)
{
    if ((flags & LOX_SORT_VERIFY_RESULT) == 0u && !LOXSORT_ENABLE_VERIFY) {
        return LOX_STATUS_OK;
    }
    return lox_verify_sorted(base, count, element_size, compare, compare_user);
}

static int lox_is_nearly_sorted(const lox_feature_vector_t *features, const lox_profile_t *profile)
{
    return features->disorder_score <= profile->near_sorted_disorder_max;
}

static int lox_is_duplicate_heavy(const lox_feature_vector_t *features, const lox_profile_t *profile, size_t count)
{
    uint8_t sign_mask = features->comparison_sign_mask;

    if (count < profile->duplicate_intro_min_count || count > profile->duplicate_intro_max_count) {
        return 0;
    }
    if (features->equal_pair_count < profile->duplicate_equal_min) {
        return 0;
    }
    if ((sign_mask & 0x3u) != 0x3u) {
        return 0;
    }
    if (features->disorder_score < profile->duplicate_disorder_min) {
        return 0;
    }
    return 1;
}

static int lox_should_prefer_chaotic_merge(
    const lox_feature_vector_t *features,
    const lox_profile_t *profile,
    size_t count,
    size_t element_size,
    int has_scratch)
{
    size_t data_bytes = 0u;
    uint8_t sign_mask = features->comparison_sign_mask;

    if (!has_scratch) {
        return 0;
    }
    if (!lox_checked_multiply_size(count, element_size, &data_bytes)) {
        return 0;
    }
    if (count < profile->chaotic_merge_min_count ||
        data_bytes < profile->chaotic_merge_min_data_bytes) {
        return 0;
    }
    if (data_bytes > profile->chaotic_merge_data_bytes_max) {
        return 0;
    }

    if ((sign_mask & 0x3u) == 0x3u &&
        features->direction_changes >= profile->chaotic_merge_direction_min &&
        features->disorder_score >= profile->chaotic_merge_disorder_min) {
        return 1;
    }

    return 0;
}

static int lox_should_prefer_large_mixed_merge(
    const lox_feature_vector_t *features,
    const lox_profile_t *profile,
    size_t count,
    size_t element_size,
    int has_scratch)
{
    size_t data_bytes = 0u;
    uint8_t sign_mask = features->comparison_sign_mask;

    if (!has_scratch) {
        return 0;
    }
    if (!lox_checked_multiply_size(count, element_size, &data_bytes)) {
        return 0;
    }
    if (count < profile->merge_large_mixed_min_count ||
        data_bytes < profile->merge_large_mixed_min_data_bytes) {
        return 0;
    }
    if ((sign_mask & 0x3u) != 0x3u) {
        return 0;
    }
    if (features->direction_changes < profile->merge_large_mixed_direction_min) {
        return 0;
    }
    if (features->direction_changes > profile->merge_large_mixed_direction_max) {
        return 0;
    }
    if (features->disorder_score < profile->merge_large_mixed_disorder_min) {
        return 0;
    }
    return 1;
}

static uint16_t lox_profile_merge_limit(const lox_profile_t *profile, uint8_t element_class)
{
    switch (element_class) {
    case LOX_ELEMENT_SIZE_SMALL:
        return profile->merge_min_count_elem_small;
    case LOX_ELEMENT_SIZE_MEDIUM:
        return profile->merge_min_count_elem_medium;
    default:
        return profile->merge_min_count_elem_large;
    }
}

static uint16_t lox_profile_shell_limit(const lox_profile_t *profile, uint8_t element_class)
{
    switch (element_class) {
    case LOX_ELEMENT_SIZE_SMALL:
        return profile->shell_max_count_elem_small;
    case LOX_ELEMENT_SIZE_MEDIUM:
        return profile->shell_max_count_elem_medium;
    default:
        return profile->shell_max_count_elem_large;
    }
}

static int lox_has_sufficient_scratch(const void *base, size_t count, size_t element_size, const lox_sort_options_t *options)
{
    size_t required = lox_merge_scratch_required(count, element_size);
    if (required == SIZE_MAX) {
        return 0;
    }
    if (options->scratch == NULL || options->scratch_size < required) {
        return 0;
    }
    if (lox_ranges_overlap(base, required, options->scratch, options->scratch_size)) {
        return 0;
    }
    return 1;
}

static lox_status_t lox_dispatch_choose_and_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options,
    const lox_profile_t *profile,
    lox_sort_result_t *result)
{
    uint16_t compiled_mask = lox_compiled_algorithm_mask();
    uint16_t effective_mask = (uint16_t)(compiled_mask & profile->algorithm_mask);
    lox_feature_vector_t features;
    uint32_t flags = options->flags;
    uint8_t element_class = lox_element_size_class(element_size);
    uint16_t shell_limit = lox_profile_shell_limit(profile, element_class);
    uint16_t merge_limit = lox_profile_merge_limit(profile, element_class);
    size_t data_bytes = count * element_size;
    const lox_algorithm_entry_t *entry = NULL;
    lox_algorithm_t chosen = LOX_ALGORITHM_NONE;
    lox_decision_reason_t reason = LOX_REASON_PROFILE_GENERAL;
    int needs_features = 1;
    int nearly_sorted = 0;
    int duplicate_heavy = 0;
    int has_scratch = 0;

    effective_mask |= LOX_ALGORITHM_MASK_INSERTION;
    if (!lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_INSERTION)) {
        return LOX_STATUS_NO_ELIGIBLE_ALGORITHM;
    }

    has_scratch = lox_has_sufficient_scratch(base, count, element_size, options);
    memset(&features, 0, sizeof(features));

    if (count <= profile->small_count_cutoff) {
        chosen = LOX_ALGORITHM_INSERTION;
        reason = LOX_REASON_SMALL_INPUT;
        needs_features = 0;
    } else {
        features = lox_extract_features(base, count, element_size, compare, compare_user, profile);
        nearly_sorted = lox_is_nearly_sorted(&features, profile);
        duplicate_heavy = lox_is_duplicate_heavy(&features, profile, count);
        if (options->flags & LOX_SORT_REQUIRE_STABLE) {
            if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_MERGE) &&
                has_scratch &&
                count >= merge_limit &&
                count >= profile->merge_min_count &&
                data_bytes >= profile->merge_min_data_bytes) {
                if (count <= profile->stable_insertion_cutoff && nearly_sorted) {
                    chosen = LOX_ALGORITHM_INSERTION;
                    reason = LOX_REASON_STABLE_WITH_SCRATCH;
                } else {
                    chosen = LOX_ALGORITHM_MERGE;
                    reason = LOX_REASON_STABLE_WITH_SCRATCH;
                }
            } else {
                chosen = LOX_ALGORITHM_INSERTION;
                reason = LOX_REASON_STABLE_FALLBACK;
            }
        } else if ((options->flags & LOX_SORT_PREFER_MIN_WRITES) != 0u &&
            lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_CYCLE) &&
            count >= profile->cycle_min_count &&
            features.equal_score <= profile->cycle_equal_score_max) {
            chosen = LOX_ALGORITHM_CYCLE;
            reason = LOX_REASON_MIN_WRITES;
        } else if (duplicate_heavy && lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_INTRO)) {
            chosen = LOX_ALGORITHM_INTRO;
            reason = LOX_REASON_PROFILE_DUPLICATE_RANGE;
        } else if (nearly_sorted && count <= profile->near_sorted_insertion_max_count) {
            if (features.direction_changes <= profile->near_sorted_direction_changes_max) {
                chosen = LOX_ALGORITHM_INSERTION;
                reason = LOX_REASON_NEARLY_SORTED;
            } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_INTRO)) {
                chosen = LOX_ALGORITHM_INTRO;
                reason = LOX_REASON_PROFILE_DIRECTION_RANGE;
            } else {
                chosen = LOX_ALGORITHM_INSERTION;
                reason = LOX_REASON_NEARLY_SORTED;
            }
        } else if (nearly_sorted && features.direction_changes > profile->near_sorted_direction_changes_max &&
            lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_INTRO)) {
            chosen = LOX_ALGORITHM_INTRO;
            reason = LOX_REASON_PROFILE_DIRECTION_RANGE;
        } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_MERGE) &&
            count >= profile->merge_min_count &&
            data_bytes >= profile->merge_min_data_bytes &&
            lox_should_prefer_large_mixed_merge(&features, profile, count, element_size, has_scratch)) {
            chosen = LOX_ALGORITHM_MERGE;
            reason = LOX_REASON_PROFILE_MERGE_RANGE;
        } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_MERGE) &&
            count >= profile->merge_min_count &&
            data_bytes >= profile->merge_min_data_bytes &&
            lox_should_prefer_chaotic_merge(&features, profile, count, element_size, has_scratch)) {
            chosen = LOX_ALGORITHM_MERGE;
            reason = LOX_REASON_PROFILE_MERGE_RANGE;
        } else if ((options->flags & LOX_SORT_NO_RECURSION) != 0u) {
            if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_SHELL) && count <= shell_limit) {
                chosen = LOX_ALGORITHM_SHELL;
                reason = LOX_REASON_NO_RECURSION;
            } else {
                chosen = LOX_ALGORITHM_INSERTION;
                reason = LOX_REASON_NO_RECURSION;
            }
        } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_MERGE) &&
            has_scratch &&
            count >= profile->merge_min_count &&
            data_bytes >= profile->merge_min_data_bytes &&
            count >= merge_limit &&
            merge_limit != 0u) {
            chosen = LOX_ALGORITHM_MERGE;
            reason = LOX_REASON_PROFILE_MERGE_RANGE;
        } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_SHELL) &&
            count <= shell_limit &&
            shell_limit != 0u) {
            chosen = LOX_ALGORITHM_SHELL;
            reason = LOX_REASON_PROFILE_SHELL_RANGE;
        } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_INTRO)) {
            chosen = LOX_ALGORITHM_INTRO;
            reason = LOX_REASON_PROFILE_GENERAL;
        } else if (lox_algorithm_enabled_by_mask(effective_mask, LOX_ALGORITHM_MASK_SHELL)) {
            chosen = LOX_ALGORITHM_SHELL;
            reason = LOX_REASON_PROFILE_GENERAL;
        } else {
            chosen = LOX_ALGORITHM_INSERTION;
            reason = LOX_REASON_PROFILE_GENERAL;
        }
    }

    if (result != NULL) {
        result->algorithm = chosen;
        result->reason = reason;
        result->profile_fallback_used = 0u;
        result->sampled_pair_count = needs_features ? features.sampled_pair_count : 0u;
        result->disorder_score = needs_features ? features.disorder_score : 0u;
        result->equal_score = needs_features ? features.equal_score : 0u;
        result->equal_pair_count = needs_features ? features.equal_pair_count : 0u;
        result->direction_changes = needs_features ? features.direction_changes : 0u;
        result->comparison_sign_mask = needs_features ? features.comparison_sign_mask : 0u;
    }

    entry = lox_pick_entry(chosen);
    if (entry == NULL) {
        return LOX_STATUS_NO_ELIGIBLE_ALGORITHM;
    }

    entry->sort(base, count, element_size, compare, compare_user, options);
    return lox_verify_if_requested(base, count, element_size, compare, compare_user, flags);
}

lox_status_t lox_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options,
    lox_sort_result_t *result)
{
    lox_sort_options_t local_options;
    lox_profile_validation_result_t profile_result;
    lox_status_t status;

    lox_init_result(result);

    local_options.scratch = NULL;
    local_options.scratch_size = 0u;
    local_options.profile = &lox_profile_generic;
    local_options.flags = lox_default_flags();

    if (options != NULL) {
        if (!lox_validate_options(options)) {
            return LOX_STATUS_INVALID_ARGUMENT;
        }
        local_options = *options;
        if (local_options.profile == NULL) {
            local_options.profile = &lox_profile_generic;
        }
        if (local_options.flags == 0u && options->flags == 0u) {
            local_options.flags = lox_default_flags();
        }
    }

    if (count == 0u) {
        if (result != NULL) {
            result->algorithm = LOX_ALGORITHM_NONE;
            result->reason = LOX_REASON_TRIVIAL_INPUT;
        }
        return LOX_STATUS_OK;
    }

    if (count == 1u) {
        if (base == NULL || element_size == 0u) {
            return LOX_STATUS_INVALID_ARGUMENT;
        }
        if (result != NULL) {
            result->algorithm = LOX_ALGORITHM_NONE;
            result->reason = LOX_REASON_TRIVIAL_INPUT;
        }
        return LOX_STATUS_OK;
    }

    if (base == NULL || element_size == 0u || compare == NULL) {
        return LOX_STATUS_INVALID_ARGUMENT;
    }
    if (!lox_checked_multiply_size(count, element_size, NULL)) {
        return LOX_STATUS_SIZE_OVERFLOW;
    }

    profile_result = lox_validate_profile(local_options.profile);
    if (!profile_result.valid) {
        return LOX_STATUS_INVALID_ARGUMENT;
    }
    status = lox_dispatch_choose_and_sort(
        base,
        count,
        element_size,
        compare,
        compare_user,
        &local_options,
        profile_result.profile,
        result);

    if (status == LOX_STATUS_OK && profile_result.used_generic_fallback && result != NULL) {
        result->profile_fallback_used = 1u;
    }

    return status;
}
