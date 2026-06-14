#include "../lox_internal.h"

#if LOXSORT_ENABLE_CYCLE

#define LOXSORT_CYCLE_TEMP_BYTES 1024u

static void lox_cycle_copy_item(unsigned char *dst, const unsigned char *src, size_t size)
{
    lox_copy_bytes(dst, src, size);
}

void lox_cycle_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options)
{
    unsigned char *bytes = (unsigned char *)base;
    size_t cycle_start;
    unsigned char temp[LOXSORT_CYCLE_TEMP_BYTES];
    (void)options;

    if (count < 2u || element_size > sizeof(temp)) {
        lox_insertion_sort(base, count, element_size, compare, compare_user, options);
        return;
    }

    for (cycle_start = 0u; cycle_start + 1u < count; ++cycle_start) {
        size_t pos = cycle_start;
        size_t i;
        size_t writes = 0u;

        lox_cycle_copy_item(temp, lox_const_element_address(bytes, cycle_start, element_size), element_size);

        for (i = cycle_start + 1u; i < count; ++i) {
            if (compare(lox_const_element_address(bytes, i, element_size), temp, compare_user) < 0) {
                ++pos;
            }
        }

        if (pos == cycle_start) {
            continue;
        }

        while (compare(lox_const_element_address(bytes, pos, element_size), temp, compare_user) == 0) {
            ++pos;
        }

        if (pos != cycle_start) {
            unsigned char next[LOXSORT_CYCLE_TEMP_BYTES];
            lox_cycle_copy_item(next, lox_const_element_address(bytes, pos, element_size), element_size);
            lox_copy_bytes(lox_element_address(bytes, pos, element_size), temp, element_size);
            ++writes;
            lox_cycle_copy_item(temp, next, element_size);
        }

        while (pos != cycle_start) {
            pos = cycle_start;
            for (i = cycle_start + 1u; i < count; ++i) {
                if (compare(lox_const_element_address(bytes, i, element_size), temp, compare_user) < 0) {
                    ++pos;
                }
            }
            while (compare(lox_const_element_address(bytes, pos, element_size), temp, compare_user) == 0) {
                ++pos;
            }
            if (pos != cycle_start) {
                unsigned char next[LOXSORT_CYCLE_TEMP_BYTES];
                lox_cycle_copy_item(next, lox_const_element_address(bytes, pos, element_size), element_size);
                lox_copy_bytes(lox_element_address(bytes, pos, element_size), temp, element_size);
                ++writes;
                lox_cycle_copy_item(temp, next, element_size);
            }
            if (writes > count) {
                break;
            }
        }
    }
}

#else

void lox_cycle_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options)
{
    (void)base;
    (void)count;
    (void)element_size;
    (void)compare;
    (void)compare_user;
    (void)options;
}

#endif

