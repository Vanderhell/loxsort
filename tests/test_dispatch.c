#include "test_support.h"
#include "../src/lox_internal.h"

void test_dispatch_suite(void)
{
    uint32_t data[64];
    lox_sort_options_t options;
    lox_sort_result_t result;
    size_t i;

    TEST_CASE("no-recursion");
    for (i = 0u; i < 64u; ++i) {
        data[i] = (uint32_t)(64u - i);
    }

    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = &lox_profile_generic;
    options.flags = LOX_SORT_NO_RECURSION;
    REQUIRE(lox_sort(data, 64u, sizeof(uint32_t), test_compare_u32, NULL, &options, &result) == LOX_STATUS_OK);
    REQUIRE(result.algorithm == LOX_ALGORITHM_SHELL || result.algorithm == LOX_ALGORITHM_INSERTION);

    TEST_CASE("general-path");
    if (lox_compiled_algorithm_mask() & LOX_ALGORITHM_MASK_INTRO) {
        for (i = 0u; i < 64u; ++i) {
            data[i] = (uint32_t)(64u - i);
        }
        options.flags = 0u;
        REQUIRE(lox_sort(data, 64u, sizeof(uint32_t), test_compare_u32, NULL, &options, &result) == LOX_STATUS_OK);
        REQUIRE(result.algorithm == LOX_ALGORITHM_INTRO || result.algorithm == LOX_ALGORITHM_SHELL || result.algorithm == LOX_ALGORITHM_MERGE);
    }

    TEST_CASE("stable-fallback");
    for (i = 0u; i < 64u; ++i) {
        data[i] = (uint32_t)(i % 3u);
    }
    options.flags = LOX_SORT_REQUIRE_STABLE;
    options.scratch = NULL;
    options.scratch_size = 0u;
    REQUIRE(lox_sort(data, 64u, sizeof(uint32_t), test_compare_u32, NULL, &options, &result) == LOX_STATUS_OK);
    REQUIRE(result.algorithm == LOX_ALGORITHM_INSERTION || result.algorithm == LOX_ALGORITHM_MERGE);
}
