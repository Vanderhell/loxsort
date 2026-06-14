#include "lox_internal.h"

lox_feature_vector_t lox_extract_features(
    const void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_profile_t *profile)
{
    lox_feature_vector_t features;
    size_t pair_count;
    size_t s;
    uint16_t descending_pairs;
    uint16_t equal_pairs;
    uint8_t direction_changes;
    int8_t last_direction;
    uint8_t comparison_sign_mask;
    uint8_t sample_pairs;

    features.count_bucket = lox_count_bucket(count);
    features.element_size_class = lox_element_size_class(element_size);
    features.disorder_score = 0u;
    features.equal_score = 0u;
    features.sampled_pair_count = 0u;
    features.equal_pair_count = 0u;
    features.direction_changes = 0u;
    features.comparison_sign_mask = 0u;

    if (profile == NULL || count < 2u) {
        return features;
    }

    sample_pairs = profile->sample_pairs;
    pair_count = count - 1u;
    if (sample_pairs < pair_count) {
        pair_count = sample_pairs;
    }

    descending_pairs = 0u;
    equal_pairs = 0u;
    direction_changes = 0u;
    last_direction = 0;
    comparison_sign_mask = 0u;
    for (s = 0u; s < pair_count; ++s) {
        size_t left = (s * (count - 1u)) / pair_count;
        size_t right = left + 1u;
        const void *lhs = lox_const_element_address(base, left, element_size);
        const void *rhs = lox_const_element_address(base, right, element_size);
        int cmp = compare(lhs, rhs, compare_user);
        int8_t current_direction = 0;

        if (cmp > 0) {
            ++descending_pairs;
            current_direction = 1;
            comparison_sign_mask |= 2u;
        } else if (cmp < 0) {
            current_direction = -1;
            comparison_sign_mask |= 1u;
        } else if (cmp == 0) {
            ++equal_pairs;
        }

        if (current_direction != 0) {
            if (last_direction != 0 && current_direction != last_direction) {
                ++direction_changes;
            }
            last_direction = current_direction;
        }
    }

    features.disorder_score = (uint8_t)((descending_pairs * 255u) / pair_count);
    features.equal_score = (uint8_t)((equal_pairs * 255u) / pair_count);
    features.sampled_pair_count = (uint8_t)pair_count;
    features.equal_pair_count = (uint8_t)equal_pairs;
    features.direction_changes = direction_changes;
    features.comparison_sign_mask = comparison_sign_mask;
    return features;
}
