#ifndef LOXSORT_INTERNAL_H
#define LOXSORT_INTERNAL_H

#include "loxsort/loxsort.h"
#include <limits.h>
#include <stdint.h>
#include <string.h>

#ifndef LOXSORT_ENABLE_SHELL
#define LOXSORT_ENABLE_SHELL 1
#endif
#ifndef LOXSORT_ENABLE_INTRO
#define LOXSORT_ENABLE_INTRO 1
#endif
#ifndef LOXSORT_ENABLE_MERGE
#define LOXSORT_ENABLE_MERGE 1
#endif
#ifndef LOXSORT_ENABLE_CYCLE
#define LOXSORT_ENABLE_CYCLE 0
#endif
#ifndef LOXSORT_ENABLE_VERIFY
#define LOXSORT_ENABLE_VERIFY 0
#endif

enum {
    LOXSORT_SORT_FLAG_MASK = LOX_SORT_REQUIRE_STABLE |
        LOX_SORT_PREFER_MIN_WRITES |
        LOX_SORT_NO_RECURSION |
        LOX_SORT_VERIFY_RESULT
};

enum lox_element_size_class_e {
    LOX_ELEMENT_SIZE_SMALL = 0,
    LOX_ELEMENT_SIZE_MEDIUM = 1,
    LOX_ELEMENT_SIZE_LARGE = 2
};

typedef struct lox_feature_vector_s {
    uint8_t count_bucket;
    uint8_t element_size_class;
    uint8_t disorder_score;
    uint8_t equal_score;
    uint8_t sampled_pair_count;
} lox_feature_vector_t;

typedef void (*lox_algorithm_fn)(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);

typedef struct lox_algorithm_entry_s {
    lox_algorithm_t id;
    uint16_t mask_bit;
    uint8_t is_stable;
    uint8_t uses_recursion;
    lox_algorithm_fn sort;
} lox_algorithm_entry_t;

typedef struct lox_profile_validation_result_s {
    const lox_profile_t *profile;
    uint8_t used_generic_fallback;
    uint8_t valid;
} lox_profile_validation_result_t;

size_t lox_checked_multiply_size(size_t left, size_t right, size_t *product);
void *lox_element_address(void *base, size_t index, size_t element_size);
const void *lox_const_element_address(const void *base, size_t index, size_t element_size);
void lox_copy_bytes(void *dest, const void *src, size_t size);
void lox_swap_bytes(void *left, void *right, size_t size);
int lox_ranges_overlap(const void *left, size_t left_size, const void *right, size_t right_size);

uint16_t lox_compiled_algorithm_mask(void);
const lox_algorithm_entry_t *lox_registry_entries(size_t *count);
const lox_algorithm_entry_t *lox_registry_find(lox_algorithm_t id);

lox_profile_validation_result_t lox_validate_profile(const lox_profile_t *profile);
lox_feature_vector_t lox_extract_features(
    const void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_profile_t *profile);

void lox_insertion_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);
void lox_shell_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);
void lox_intro_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);
void lox_merge_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);
void lox_cycle_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);

lox_status_t lox_verify_sorted(
    const void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user);

static inline uint8_t lox_element_size_class(size_t element_size)
{
    if (element_size <= 4u) {
        return LOX_ELEMENT_SIZE_SMALL;
    }
    if (element_size <= 16u) {
        return LOX_ELEMENT_SIZE_MEDIUM;
    }
    return LOX_ELEMENT_SIZE_LARGE;
}

static inline uint8_t lox_count_bucket(size_t count)
{
    if (count <= 1u) {
        return 0u;
    }
    if (count <= 3u) {
        return 1u;
    }
    if (count <= 7u) {
        return 2u;
    }
    if (count <= 15u) {
        return 3u;
    }
    if (count <= 31u) {
        return 4u;
    }
    if (count <= 63u) {
        return 5u;
    }
    if (count <= 127u) {
        return 6u;
    }
    return 7u;
}

static inline uint32_t lox_default_flags(void)
{
#if LOXSORT_ENABLE_VERIFY
    return LOX_SORT_VERIFY_RESULT;
#else
    return 0u;
#endif
}

#endif

