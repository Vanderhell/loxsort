#include "test_support.h"
#include "../src/lox_internal.h"

#include <string.h>

typedef void (*sort_fn_t)(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options);

typedef struct sort_case_s {
    const char *name;
    sort_fn_t sort;
    int stable;
    int needs_scratch;
} sort_case_t;

static const sort_case_t g_cases[] = {
    { "insertion", lox_insertion_sort, 1, 0 },
    { "shell", lox_shell_sort, 0, 0 },
    { "intro", lox_intro_sort, 0, 0 },
    { "merge", lox_merge_sort, 1, 1 }
};

static void test_fill_pattern(unsigned char *items, size_t count, size_t element_size, size_t key_size, int pattern)
{
    size_t i;

    for (i = 0u; i < count; ++i) {
        unsigned char *slot = items + (i * element_size);
        size_t j;
        memset(slot, 0, element_size);
        switch (pattern) {
        case 0:
            for (j = 0u; j < key_size; ++j) {
                slot[j] = (unsigned char)((i >> (j * 8u)) & 0xffu);
            }
            break;
        case 1:
            for (j = 0u; j < key_size; ++j) {
                slot[j] = (unsigned char)(((count - i) >> (j * 8u)) & 0xffu);
            }
            break;
        case 2:
            for (j = 0u; j < key_size; ++j) {
                slot[j] = 1u;
            }
            break;
        case 3:
            for (j = 0u; j < key_size; ++j) {
                slot[j] = (unsigned char)((i % 2u) ? 0x11u : 0x22u);
            }
            break;
        default:
            for (j = 0u; j < key_size; ++j) {
                slot[j] = (unsigned char)(((i * 13u) + j * 7u) & 0xffu);
            }
            break;
        }
        for (j = key_size; j < element_size; ++j) {
            slot[j] = (unsigned char)((i * 17u + j) & 0xffu);
        }
    }
}

static void test_reference_sort(unsigned char *items, size_t count, size_t element_size, size_t key_size)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        size_t j = i;
        while (j > 0u &&
            test_compare_raw_key(items + ((j - 1u) * element_size), items + (j * element_size), &key_size) > 0) {
            unsigned char temp[32];
            memcpy(temp, items + ((j - 1u) * element_size), element_size);
            memcpy(items + ((j - 1u) * element_size), items + (j * element_size), element_size);
            memcpy(items + (j * element_size), temp, element_size);
            --j;
        }
    }
}

static uint64_t test_fingerprint(const unsigned char *items, size_t count, size_t element_size)
{
    size_t i;
    uint64_t hash = 1469598103934665603ull;
    for (i = 0u; i < count * element_size; ++i) {
        hash ^= (uint64_t)items[i];
        hash *= 1099511628211ull;
    }
    return hash;
}

static void test_run_algorithm_case(
    const sort_case_t *c,
    unsigned char *actual,
    size_t count,
    size_t element_size,
    size_t key_size)
{
    unsigned char expected[32 * 32];
    lox_sort_options_t options;

    REQUIRE(element_size <= 32u);
    REQUIRE(count * element_size <= sizeof(expected));

    memcpy(expected, actual, count * element_size);
    test_reference_sort(expected, count, element_size, key_size);

    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = &lox_profile_generic;
    options.flags = 0u;
    if (c->needs_scratch) {
        static unsigned char scratch[4096];
        size_t required = lox_merge_scratch_required(count, element_size);
        REQUIRE(required != SIZE_MAX);
        REQUIRE(required <= sizeof(scratch));
        options.scratch = scratch;
        options.scratch_size = required;
    }

    c->sort(actual, count, element_size, test_compare_raw_key, &key_size, &options);
    REQUIRE(test_is_sorted_raw(actual, count, element_size, key_size));
    REQUIRE(test_fingerprint(actual, count, element_size) == test_fingerprint(expected, count, element_size));
    if (c->stable) {
        REQUIRE(memcmp(actual, expected, count * element_size) == 0);
    }
}

static void test_exhaustive_small(const sort_case_t *c)
{
    size_t n;
    size_t element_size = sizeof(uint32_t);
    size_t key_size = sizeof(uint32_t);
    unsigned char items[6 * sizeof(uint32_t)];
    size_t total;

    for (n = 0u; n <= 6u; ++n) {
        total = 1u;
        for (size_t i = 0u; i < n; ++i) {
            total *= 4u;
        }
        for (size_t mask = 0u; mask < total; ++mask) {
            size_t tmp = mask;
            size_t i;
            for (i = 0u; i < n; ++i) {
                uint32_t key = (uint32_t)(tmp % 4u);
                tmp /= 4u;
                memcpy(items + (i * element_size), &key, sizeof(key));
                memset(items + (i * element_size) + sizeof(key), (int)(i & 0xffu), element_size - sizeof(key));
            }
            test_run_algorithm_case(c, items, n, element_size, key_size);
        }
    }
}

void test_algorithms_suite(void)
{
    size_t i;
    size_t element_sizes[] = { 1u, 2u, 3u, 4u, 7u, 8u, 16u, 31u, 32u };

    for (i = 0u; i < sizeof(g_cases) / sizeof(g_cases[0]); ++i) {
        TEST_CASE(g_cases[i].name);
        test_exhaustive_small(&g_cases[i]);
    }

    for (i = 0u; i < sizeof(g_cases) / sizeof(g_cases[0]); ++i) {
        size_t es;
        TEST_CASE("property-matrix");
        for (es = 0u; es < sizeof(element_sizes) / sizeof(element_sizes[0]); ++es) {
            size_t pattern;
            for (pattern = 0u; pattern < 5u; ++pattern) {
                size_t count = 8u + es;
                size_t key_size = element_sizes[es] < sizeof(uint32_t) ? element_sizes[es] : sizeof(uint32_t);
                unsigned char items[32 * 32];
                test_fill_pattern(items, count, element_sizes[es], key_size, (int)pattern);
                test_run_algorithm_case(&g_cases[i], items, count, element_sizes[es], key_size);
            }
        }
    }

    TEST_CASE("merge-scratch");
    if (lox_compiled_algorithm_mask() & LOX_ALGORITHM_MASK_MERGE) {
        unsigned char items[16 * 8];
        size_t key_size = 4u;
        test_fill_pattern(items, 16u, 8u, key_size, 4);
        test_run_algorithm_case(&g_cases[3], items, 16u, 8u, key_size);
    }

    if (lox_compiled_algorithm_mask() & LOX_ALGORITHM_MASK_CYCLE) {
        TEST_CASE("cycle-duplicates");
        {
            unsigned char items[16 * 8];
            size_t key_size = 4u;
            lox_sort_options_t options;
            size_t count = 16u;
            test_fill_pattern(items, count, 8u, key_size, 2);
            options.scratch = NULL;
            options.scratch_size = 0u;
            options.profile = &lox_profile_generic;
            options.flags = LOX_SORT_PREFER_MIN_WRITES;
            lox_cycle_sort(items, count, 8u, test_compare_raw_key, &key_size, &options);
            REQUIRE(test_is_sorted_raw(items, count, 8u, key_size));
        }
    }
}
