#include "test_support.h"
#include "../src/lox_internal.h"

void test_profile_suite(void)
{
    lox_profile_validation_result_t valid;
    lox_profile_t profile = lox_profile_generic;

    TEST_CASE("generic-valid");
    valid = lox_validate_profile(&profile);
    REQUIRE(valid.valid != 0u);
    REQUIRE(valid.used_generic_fallback == 0u);
    REQUIRE(valid.profile == &profile);

    TEST_CASE("format-fallback");
    profile.format_version = 99u;
    valid = lox_validate_profile(&profile);
    REQUIRE(valid.used_generic_fallback != 0u);
    REQUIRE(valid.profile == &lox_profile_generic);

    TEST_CASE("sample-pairs-fallback");
    profile = lox_profile_generic;
    profile.sample_pairs = 0u;
    valid = lox_validate_profile(&profile);
    REQUIRE(valid.used_generic_fallback != 0u);

    TEST_CASE("insertion-required");
    profile = lox_profile_generic;
    profile.algorithm_mask &= (uint16_t)~LOX_ALGORITHM_MASK_INSERTION;
    valid = lox_validate_profile(&profile);
    REQUIRE(valid.used_generic_fallback != 0u);
}
