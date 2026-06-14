#include "test_support.h"
#include "../src/lox_internal.h"

void test_stability_suite(void)
{
    test_item_t items[10];
    lox_sort_options_t options;
    lox_sort_result_t result;

    TEST_CASE("insertion-stable");
    test_make_items(items, 10u, 123u, 3u);

    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = &lox_profile_generic;
    options.flags = LOX_SORT_REQUIRE_STABLE;
    REQUIRE(lox_sort(items, 10u, sizeof(test_item_t), test_compare_item_key, NULL, &options, &result) == LOX_STATUS_OK);
    REQUIRE(test_is_stable(items, 10u));

    TEST_CASE("merge-stable");
    if (lox_compiled_algorithm_mask() & LOX_ALGORITHM_MASK_MERGE) {
        size_t scratch_size = lox_merge_scratch_required(10u, sizeof(test_item_t));
        unsigned char scratch[1024];
        test_make_items(items, 10u, 123u, 3u);
        options.scratch = scratch;
        options.scratch_size = scratch_size;
        options.flags = LOX_SORT_REQUIRE_STABLE;
        REQUIRE(lox_sort(items, 10u, sizeof(test_item_t), test_compare_item_key, NULL, &options, &result) == LOX_STATUS_OK);
        REQUIRE(test_is_stable(items, 10u));
    }
}
