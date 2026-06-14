#include "test_support.h"
#include "../src/lox_internal.h"

void test_api_suite(void)
{
    lox_sort_result_t result;
    uint32_t data[4] = { 4u, 3u, 2u, 1u };
    lox_sort_options_t options;
    lox_status_t status;
    lox_profile_t invalid_profile;

    TEST_CASE("invalid-arguments");
    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = &lox_profile_generic;
    options.flags = 0u;

    status = lox_sort(NULL, 0u, 0u, NULL, NULL, NULL, &result);
    REQUIRE(status == LOX_STATUS_OK);
    REQUIRE(result.algorithm == LOX_ALGORITHM_NONE);

    status = lox_sort(NULL, 1u, sizeof(uint32_t), test_compare_u32, NULL, NULL, &result);
    REQUIRE(status == LOX_STATUS_INVALID_ARGUMENT);

    status = lox_sort(data, 4u, 0u, test_compare_u32, NULL, &options, &result);
    REQUIRE(status == LOX_STATUS_INVALID_ARGUMENT);

    status = lox_sort(data, 4u, sizeof(uint32_t), NULL, NULL, &options, &result);
    REQUIRE(status == LOX_STATUS_INVALID_ARGUMENT);

    options.flags = 0x80000000u;
    status = lox_sort(data, 4u, sizeof(uint32_t), test_compare_u32, NULL, &options, &result);
    REQUIRE(status == LOX_STATUS_INVALID_ARGUMENT);

    options.flags = 0u;
    options.scratch_size = 8u;
    status = lox_sort(data, 4u, sizeof(uint32_t), test_compare_u32, NULL, &options, &result);
    REQUIRE(status == LOX_STATUS_INVALID_ARGUMENT);

    TEST_CASE("profile-fallback");
    options.scratch_size = 0u;
    invalid_profile = lox_profile_generic;
    invalid_profile.format_version = 99u;
    options.profile = &invalid_profile;
    status = lox_sort(data, 4u, sizeof(uint32_t), test_compare_u32, NULL, &options, &result);
    REQUIRE(status == LOX_STATUS_OK);
    REQUIRE(result.profile_fallback_used != 0u);

    TEST_CASE("helpers");
    REQUIRE(lox_merge_scratch_required(SIZE_MAX, 2u) == SIZE_MAX);
    REQUIRE(strcmp(lox_status_name(LOX_STATUS_OK), "ok") == 0);
    REQUIRE(strcmp(lox_algorithm_name(LOX_ALGORITHM_INSERTION), "insertion") == 0);
}
