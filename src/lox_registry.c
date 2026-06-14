#include "lox_internal.h"

static const lox_algorithm_entry_t g_entries[] = {
    { LOX_ALGORITHM_INSERTION, LOX_ALGORITHM_MASK_INSERTION, 1u, 0u, lox_insertion_sort },
#if LOXSORT_ENABLE_SHELL
    { LOX_ALGORITHM_SHELL, LOX_ALGORITHM_MASK_SHELL, 0u, 0u, lox_shell_sort },
#endif
#if LOXSORT_ENABLE_INTRO
    { LOX_ALGORITHM_INTRO, LOX_ALGORITHM_MASK_INTRO, 0u, 1u, lox_intro_sort },
#endif
#if LOXSORT_ENABLE_MERGE
    { LOX_ALGORITHM_MERGE, LOX_ALGORITHM_MASK_MERGE, 1u, 0u, lox_merge_sort },
#endif
#if LOXSORT_ENABLE_CYCLE
    { LOX_ALGORITHM_CYCLE, LOX_ALGORITHM_MASK_CYCLE, 0u, 0u, lox_cycle_sort },
#endif
};

uint16_t lox_compiled_algorithm_mask(void)
{
    uint16_t mask = 0u;
    size_t i;

    for (i = 0u; i < sizeof(g_entries) / sizeof(g_entries[0]); ++i) {
        mask = (uint16_t)(mask | g_entries[i].mask_bit);
    }
    return mask;
}

const lox_algorithm_entry_t *lox_registry_entries(size_t *count)
{
    if (count != NULL) {
        *count = sizeof(g_entries) / sizeof(g_entries[0]);
    }
    return g_entries;
}

const lox_algorithm_entry_t *lox_registry_find(lox_algorithm_t id)
{
    size_t i;

    for (i = 0u; i < sizeof(g_entries) / sizeof(g_entries[0]); ++i) {
        if (g_entries[i].id == id) {
            return &g_entries[i];
        }
    }
    return NULL;
}

