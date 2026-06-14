#include "../lox_internal.h"

static void lox_merge_pass(
    unsigned char *dest,
    const unsigned char *src,
    size_t left,
    size_t mid,
    size_t right,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user)
{
    size_t i = left;
    size_t j = mid;
    size_t k = left;

    while (i < mid && j < right) {
        const void *lhs = lox_const_element_address(src, i, element_size);
        const void *rhs = lox_const_element_address(src, j, element_size);
        if (compare(lhs, rhs, compare_user) <= 0) {
            lox_copy_bytes(lox_element_address(dest, k, element_size), lhs, element_size);
            ++i;
        } else {
            lox_copy_bytes(lox_element_address(dest, k, element_size), rhs, element_size);
            ++j;
        }
        ++k;
    }

    while (i < mid) {
        lox_copy_bytes(
            lox_element_address(dest, k, element_size),
            lox_const_element_address(src, i, element_size),
            element_size);
        ++i;
        ++k;
    }

    while (j < right) {
        lox_copy_bytes(
            lox_element_address(dest, k, element_size),
            lox_const_element_address(src, j, element_size),
            element_size);
        ++j;
        ++k;
    }
}

void lox_merge_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options)
{
    unsigned char *work;
    unsigned char *scratch;
    size_t width;
    int flip = 0;
    (void)compare_user;

    if (count < 2u) {
        return;
    }

    scratch = (unsigned char *)options->scratch;
    work = (unsigned char *)base;

    for (width = 1u; width < count; width *= 2u) {
        size_t left;
        unsigned char *src;
        unsigned char *dest;

        src = flip ? scratch : work;
        dest = flip ? work : scratch;

        for (left = 0u; left < count; left += width * 2u) {
            size_t mid = left + width;
            size_t right = left + width * 2u;
            if (mid > count) {
                mid = count;
            }
            if (right > count) {
                right = count;
            }
            if (mid == right) {
                lox_copy_bytes(
                    lox_element_address(dest, left, element_size),
                    lox_const_element_address(src, left, element_size),
                    (right - left) * element_size);
                continue;
            }
            lox_merge_pass(dest, src, left, mid, right, element_size, compare, compare_user);
        }
        flip = !flip;
    }

    if (flip) {
        lox_copy_bytes(base, scratch, count * element_size);
    }
}

