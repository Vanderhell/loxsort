#include "lox_clock.h"

#ifndef _WIN32
#include <time.h>

static uint64_t lox_clock_posix_now(void)
{
    struct timespec ts;
#if defined(CLOCK_MONOTONIC)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

static const lox_clock_tick_source_t g_clock = { lox_clock_posix_now, 1000000000ull };

const lox_clock_tick_source_t *lox_clock_default(void)
{
    return &g_clock;
}

#endif

