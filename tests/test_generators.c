#include "test_support.h"

void test_generators_suite(void)
{
    uint32_t a[8];
    uint32_t b[8];

    TEST_CASE("deterministic");
    test_fill_u32(a, 8u, 1u);
    test_fill_u32(b, 8u, 1u);
    REQUIRE(memcmp(a, b, sizeof(a)) == 0);
}
