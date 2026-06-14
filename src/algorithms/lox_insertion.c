#include "../lox_internal.h"

void lox_insertion_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options)
{
    unsigned char *bytes = (unsigned char *)base;
    size_t i;
    (void)options;

    for (i = 1u; i < count; ++i) {
        size_t j = i;
        while (j > 0u) {
            void *left = lox_element_address(bytes, j - 1u, element_size);
            void *right = lox_element_address(bytes, j, element_size);
            if (compare(left, right, compare_user) <= 0) {
                break;
            }
            lox_swap_bytes(left, right, element_size);
            --j;
        }
    }
}

