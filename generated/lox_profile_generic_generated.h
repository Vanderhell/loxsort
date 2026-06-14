#ifndef LOXSORT_GENERATED_LOX_PROFILE_GENERIC_GENERATED_H
#define LOXSORT_GENERATED_LOX_PROFILE_GENERIC_GENERATED_H

#include "loxsort/loxsort.h"

const lox_profile_t lox_profile_generic = {
    LOX_PROFILE_FORMAT_VERSION,
    LOX_ALGORITHM_MASK_INSERTION |
#if LOXSORT_ENABLE_SHELL
        LOX_ALGORITHM_MASK_SHELL |
#endif
#if LOXSORT_ENABLE_INTRO
        LOX_ALGORITHM_MASK_INTRO |
#endif
#if LOXSORT_ENABLE_MERGE
        LOX_ALGORITHM_MASK_MERGE |
#endif
#if LOXSORT_ENABLE_CYCLE
        LOX_ALGORITHM_MASK_CYCLE |
#endif
        0u,
    16u,
    16u,
    64u,
    32u,
    64u,
    64u,
    256u,
    512u,
    1024u,
    0u,
    0u,
    0u,
    16u,
    32u,
    4u,
    6u,
    96u,
    32u,
    0u
};

#endif
