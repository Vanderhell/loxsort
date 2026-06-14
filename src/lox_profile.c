#include "lox_internal.h"

#include "../generated/lox_profile_generic_generated.h"

static uint8_t lox_profile_algorithm_mask_valid(uint16_t mask)
{
    uint16_t allowed = LOX_ALGORITHM_MASK_INSERTION;
#if LOXSORT_ENABLE_SHELL
    allowed = (uint16_t)(allowed | LOX_ALGORITHM_MASK_SHELL);
#endif
#if LOXSORT_ENABLE_INTRO
    allowed = (uint16_t)(allowed | LOX_ALGORITHM_MASK_INTRO);
#endif
#if LOXSORT_ENABLE_MERGE
    allowed = (uint16_t)(allowed | LOX_ALGORITHM_MASK_MERGE);
#endif
#if LOXSORT_ENABLE_CYCLE
    allowed = (uint16_t)(allowed | LOX_ALGORITHM_MASK_CYCLE);
#endif
    return (uint8_t)((mask & (uint16_t)~allowed) == 0u);
}

lox_profile_validation_result_t lox_validate_profile(const lox_profile_t *profile)
{
    lox_profile_validation_result_t result;

    result.profile = &lox_profile_generic;
    result.used_generic_fallback = 1u;
    result.valid = 0u;

    if (profile == NULL) {
        result.valid = 1u;
        return result;
    }

    if (profile->format_version != LOX_PROFILE_FORMAT_VERSION) {
        result.valid = 1u;
        return result;
    }

    if (profile->sample_pairs == 0u || profile->sample_pairs > LOX_PROFILE_MAX_SAMPLE_PAIRS) {
        result.valid = 1u;
        return result;
    }

    if ((profile->algorithm_mask & LOX_ALGORITHM_MASK_INSERTION) == 0u) {
        result.valid = 1u;
        return result;
    }

    if (!lox_profile_algorithm_mask_valid(profile->algorithm_mask)) {
        result.valid = 1u;
        return result;
    }

    result.profile = profile;
    result.used_generic_fallback = 0u;
    result.valid = 1u;
    return result;
}

size_t lox_merge_scratch_required(size_t count, size_t element_size)
{
    size_t required = 0u;

    if (!lox_checked_multiply_size(count, element_size, &required)) {
        return SIZE_MAX;
    }
    return required;
}

const char *lox_algorithm_name(lox_algorithm_t algorithm)
{
    switch (algorithm) {
    case LOX_ALGORITHM_NONE:
        return "none";
    case LOX_ALGORITHM_INSERTION:
        return "insertion";
    case LOX_ALGORITHM_SHELL:
        return "shell";
    case LOX_ALGORITHM_INTRO:
        return "intro";
    case LOX_ALGORITHM_MERGE:
        return "merge";
    case LOX_ALGORITHM_CYCLE:
        return "cycle";
    default:
        return "unknown";
    }
}

const char *lox_status_name(lox_status_t status)
{
    switch (status) {
    case LOX_STATUS_OK:
        return "ok";
    case LOX_STATUS_INVALID_ARGUMENT:
        return "invalid_argument";
    case LOX_STATUS_SIZE_OVERFLOW:
        return "size_overflow";
    case LOX_STATUS_NO_ELIGIBLE_ALGORITHM:
        return "no_eligible_algorithm";
    case LOX_STATUS_VERIFY_FAILED:
        return "verify_failed";
    default:
        return "unknown";
    }
}

