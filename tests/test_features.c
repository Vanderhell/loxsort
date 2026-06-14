#include "test_support.h"
#include "../src/lox_internal.h"

static size_t g_compare_calls;

static int test_counting_compare(const void *lhs, const void *rhs, void *user)
{
    ++g_compare_calls;
    return test_compare_u32(lhs, rhs, user);
}

void test_features_suite(void)
{
    uint32_t data[8] = { 7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u };
    lox_feature_vector_t features;

    TEST_CASE("disorder-equal-scores");
    features = lox_extract_features(data, 8u, sizeof(uint32_t), test_compare_u32, NULL, &lox_profile_generic);
    REQUIRE(features.sampled_pair_count > 0u);
    REQUIRE(features.disorder_score > 0u);
    REQUIRE(features.direction_changes == 0u);
    REQUIRE((features.comparison_sign_mask & 2u) != 0u);

    data[0] = 0u;
    data[1] = 1u;
    data[2] = 2u;
    data[3] = 3u;
    data[4] = 4u;
    data[5] = 5u;
    data[6] = 6u;
    data[7] = 7u;
    features = lox_extract_features(data, 8u, sizeof(uint32_t), test_compare_u32, NULL, &lox_profile_generic);
    REQUIRE(features.disorder_score == 0u);
    REQUIRE(features.equal_score == 0u);
    REQUIRE(features.equal_pair_count == 0u);
    REQUIRE(features.direction_changes == 0u);
    REQUIRE((features.comparison_sign_mask & 1u) != 0u);
    REQUIRE((features.comparison_sign_mask & 2u) == 0u);

    data[0] = 9u;
    data[1] = 9u;
    data[2] = 9u;
    data[3] = 9u;
    data[4] = 9u;
    data[5] = 9u;
    data[6] = 9u;
    data[7] = 9u;
    features = lox_extract_features(data, 8u, sizeof(uint32_t), test_compare_u32, NULL, &lox_profile_generic);
    REQUIRE(features.equal_pair_count > 0u);
    REQUIRE(features.direction_changes == 0u);
    REQUIRE(features.comparison_sign_mask == 0u);

    data[0] = 0u;
    data[1] = 2u;
    data[2] = 1u;
    data[3] = 3u;
    data[4] = 2u;
    data[5] = 4u;
    data[6] = 3u;
    data[7] = 5u;
    features = lox_extract_features(data, 8u, sizeof(uint32_t), test_compare_u32, NULL, &lox_profile_generic);
    REQUIRE(features.equal_pair_count == 0u);
    REQUIRE((features.comparison_sign_mask & 3u) == 3u);
    REQUIRE(features.direction_changes > 0u);

    TEST_CASE("comparison-budget");
    g_compare_calls = 0u;
    features = lox_extract_features(data, 8u, sizeof(uint32_t), test_counting_compare, NULL, &lox_profile_generic);
    REQUIRE(g_compare_calls == features.sampled_pair_count);
    REQUIRE(g_compare_calls <= LOX_PROFILE_MAX_SAMPLE_PAIRS);
}
