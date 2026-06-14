#include "lox_internal.h"

lox_status_t lox_verify_sorted(
    const void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user)
{
    size_t i;

    if (count < 2u) {
        return LOX_STATUS_OK;
    }

    for (i = 1u; i < count; ++i) {
        const void *left = lox_const_element_address(base, i - 1u, element_size);
        const void *right = lox_const_element_address(base, i, element_size);
        if (compare(left, right, compare_user) > 0) {
            return LOX_STATUS_VERIFY_FAILED;
        }
    }

    return LOX_STATUS_OK;
}

