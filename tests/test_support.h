#ifndef LOXSORT_TEST_SUPPORT_H
#define LOXSORT_TEST_SUPPORT_H

#include "loxsort/loxsort.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct test_item_s {
    uint32_t key;
    uint32_t index;
    unsigned char payload[24];
} test_item_t;

int test_compare_u32(const void *lhs, const void *rhs, void *user);
int test_compare_item_key(const void *lhs, const void *rhs, void *user);
int test_compare_raw_key(const void *lhs, const void *rhs, void *user);
void test_fill_u32(uint32_t *items, size_t count, uint32_t seed);
void test_make_items(test_item_t *items, size_t count, uint32_t seed, uint32_t duplicates);
int test_is_sorted_u32(const uint32_t *items, size_t count);
int test_is_stable(const test_item_t *items, size_t count);
int test_is_sorted_raw(const unsigned char *items, size_t count, size_t element_size, size_t key_size);
void test_require(int condition, const char *expr, const char *file, int line);
void test_suite_begin(const char *name);
void test_case_begin(const char *name);
size_t test_suite_count(void);
size_t test_case_count(void);
size_t test_assertion_count(void);
void test_summary_print(void);

#define REQUIRE(expr) test_require((expr) != 0, #expr, __FILE__, __LINE__)
#define TEST_SUITE(name) test_suite_begin(name)
#define TEST_CASE(name) test_case_begin(name)

extern void test_api_suite(void);
extern void test_algorithms_suite(void);
extern void test_dispatch_suite(void);
extern void test_features_suite(void);
extern void test_profile_suite(void);
extern void test_stability_suite(void);
extern void test_generators_suite(void);

#endif
