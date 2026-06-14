#include "test_support.h"

int main(void)
{
    TEST_SUITE("api");
    test_api_suite();
    TEST_SUITE("profile");
    test_profile_suite();
    TEST_SUITE("features");
    test_features_suite();
    TEST_SUITE("algorithms");
    test_algorithms_suite();
    TEST_SUITE("stability");
    test_stability_suite();
    TEST_SUITE("dispatch");
    test_dispatch_suite();
    TEST_SUITE("generators");
    test_generators_suite();
    test_summary_print();
    return 0;
}
