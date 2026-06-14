#include "../src/lox_internal.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

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

static void fuzz_run_sort(
    void (*sort_fn)(void *, size_t, size_t, lox_compare_fn, void *, const lox_sort_options_t *),
    unsigned char *buffer,
    size_t count,
    size_t element_size)
{
    lox_sort_options_t options;
    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = &lox_profile_generic;
    options.flags = 0u;
    sort_fn(buffer, count, element_size, fuzz_compare_u8, NULL, &options);
}

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size)
{
    unsigned char buffer[64];
    size_t count;
    size_t element_size;

    if (size < 2u) {
        return 0;
    }

    count = (size_t)(data[0] % 16u);
    element_size = (size_t)((data[1] % 4u) + 1u);
    if (count == 0u || count * element_size > sizeof(buffer)) {
        return 0;
    }

    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, data + 2u, (size - 2u) < (count * element_size) ? (size - 2u) : (count * element_size));

    fuzz_run_sort(lox_insertion_sort, buffer, count, element_size);
    fuzz_run_sort(lox_shell_sort, buffer, count, element_size);
    fuzz_run_sort(lox_intro_sort, buffer, count, element_size);
    if (lox_compiled_algorithm_mask() & LOX_ALGORITHM_MASK_MERGE) {
        unsigned char scratch[256];
        lox_sort_options_t options;
        options.scratch = scratch;
        options.scratch_size = lox_merge_scratch_required(count, element_size);
        options.profile = &lox_profile_generic;
        options.flags = 0u;
        if (options.scratch_size != SIZE_MAX && options.scratch_size <= sizeof(scratch)) {
            lox_merge_sort(buffer, count, element_size, fuzz_compare_u8, NULL, &options);
        }
    }
    if (lox_compiled_algorithm_mask() & LOX_ALGORITHM_MASK_CYCLE) {
        fuzz_run_sort(lox_cycle_sort, buffer, count, element_size);
    }
    return 0;
}

