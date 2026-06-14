#include "test_support.h"

#include <string.h>

static size_t g_suite_count;
static size_t g_case_count;
static size_t g_assertion_count;

int test_compare_raw_key(const void *lhs, const void *rhs, void *user)
{
    const unsigned char *a = (const unsigned char *)lhs;
    const unsigned char *b = (const unsigned char *)rhs;
    size_t key_size = *(const size_t *)user;
    size_t i;

    for (i = 0u; i < key_size; ++i) {
        if (a[i] < b[i]) {
            return -1;
        }
        if (a[i] > b[i]) {
            return 1;
        }
    }
    return 0;
}

int test_compare_u32(const void *lhs, const void *rhs, void *user)
{
    const uint32_t *a = (const uint32_t *)lhs;
    const uint32_t *b = (const uint32_t *)rhs;
    (void)user;
    if (*a < *b) {
        return -7;
    }
    if (*a > *b) {
        return 3;
    }
    return 0;
}

int test_compare_item_key(const void *lhs, const void *rhs, void *user)
{
    const test_item_t *a = (const test_item_t *)lhs;
    const test_item_t *b = (const test_item_t *)rhs;
    (void)user;
    if (a->key < b->key) {
        return -1;
    }
    if (a->key > b->key) {
        return 1;
    }
    return 0;
}

void test_fill_u32(uint32_t *items, size_t count, uint32_t seed)
{
    size_t i;
    uint32_t value = seed;
    for (i = 0u; i < count; ++i) {
        value = value * 1664525u + 1013904223u;
        items[i] = value;
    }
}

void test_make_items(test_item_t *items, size_t count, uint32_t seed, uint32_t duplicates)
{
    size_t i;
    uint32_t value = seed;
    for (i = 0u; i < count; ++i) {
        value = value * 1103515245u + 12345u;
        items[i].key = duplicates == 0u ? value : (value % duplicates);
        items[i].index = (uint32_t)i;
        memset(items[i].payload, (int)(i & 0xffu), sizeof(items[i].payload));
    }
}

int test_is_sorted_u32(const uint32_t *items, size_t count)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        if (items[i - 1u] > items[i]) {
            return 0;
        }
    }
    return 1;
}

int test_is_stable(const test_item_t *items, size_t count)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        if (items[i - 1u].key == items[i].key &&
            items[i - 1u].index > items[i].index) {
            return 0;
        }
    }
    return 1;
}

int test_is_sorted_raw(const unsigned char *items, size_t count, size_t element_size, size_t key_size)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        if (test_compare_raw_key(items + ((i - 1u) * element_size), items + (i * element_size), &key_size) > 0) {
            return 0;
        }
    }
    return 1;
}

void test_require(int condition, const char *expr, const char *file, int line)
{
    ++g_assertion_count;
    if (!condition) {
        fprintf(stderr, "TEST FAIL %s:%d: %s\n", file, line, expr);
        abort();
    }
}

void test_suite_begin(const char *name)
{
    ++g_suite_count;
    printf("SUITE %s\n", name);
}

void test_case_begin(const char *name)
{
    ++g_case_count;
    printf("  CASE %s\n", name);
}

size_t test_suite_count(void)
{
    return g_suite_count;
}

size_t test_case_count(void)
{
    return g_case_count;
}

size_t test_assertion_count(void)
{
    return g_assertion_count;
}

void test_summary_print(void)
{
    printf("SUMMARY suites=%zu cases=%zu assertions=%zu\n",
        g_suite_count,
        g_case_count,
        g_assertion_count);
}
