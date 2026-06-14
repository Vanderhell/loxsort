#include "loxsort/loxsort.h"

#include <stddef.h>
#include <stdint.h>

static int fuzz_compare_u8(const void *lhs, const void *rhs, void *user)
{
    const uint8_t *a = (const uint8_t *)lhs;
    const uint8_t *b = (const uint8_t *)rhs;
    (void)user;
    if (*a < *b) {
        return -1;
    }
    if (*a > *b) {
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size)
{
    uint8_t values[32];
    lox_sort_options_t options;
    lox_sort_result_t result;
    size_t count;
    size_t element_size;
    size_t i;

    if (size < 3u) {
        return 0;
    }

    count = (size_t)(data[0] % 16u);
    element_size = (size_t)((data[1] % 8u) + 1u);
    if (count == 0u || count * element_size > sizeof(values) || 2u + count > size) {
        return 0;
    }

    for (i = 0u; i < count * element_size; ++i) {
        values[i] = data[2u + (i % (size - 2u))];
    }

    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = NULL;
    options.flags = (uint32_t)(data[2u] & 0x0fu);
    (void)lox_sort(values, count, element_size, fuzz_compare_u8, NULL, &options, &result);
    return 0;
}

