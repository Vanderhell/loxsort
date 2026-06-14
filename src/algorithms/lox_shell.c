#include "../lox_internal.h"

static const size_t g_ciura_base_gaps[] = { 1u, 4u, 10u, 23u, 57u, 132u, 301u, 701u };

void lox_shell_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options)
{
    unsigned char *bytes = (unsigned char *)base;
    size_t gaps[32];
    size_t gap_count = 0u;
    size_t i;
    (void)options;

    if (count < 2u) {
        return;
    }

    for (i = 0u; i < sizeof(g_ciura_base_gaps) / sizeof(g_ciura_base_gaps[0]); ++i) {
        if (g_ciura_base_gaps[i] < count) {
            gaps[gap_count++] = g_ciura_base_gaps[i];
        }
    }

    if (gap_count == 0u) {
        gaps[gap_count++] = 1u;
    }

    while (gaps[gap_count - 1u] < count) {
        size_t next = (gaps[gap_count - 1u] * 225u) / 100u;
        if (next <= gaps[gap_count - 1u]) {
            break;
        }
        if (gap_count < sizeof(gaps) / sizeof(gaps[0])) {
            gaps[gap_count++] = next;
        } else {
            break;
        }
    }

    while (gap_count > 0u) {
        size_t gap = gaps[--gap_count];
        if (gap >= count) {
            continue;
        }
        for (i = gap; i < count; ++i) {
            size_t j = i;
            while (j >= gap) {
                void *left = lox_element_address(bytes, j - gap, element_size);
                void *right = lox_element_address(bytes, j, element_size);
                if (compare(left, right, compare_user) <= 0) {
                    break;
                }
                lox_swap_bytes(left, right, element_size);
                j -= gap;
            }
        }
    }
}
