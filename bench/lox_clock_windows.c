#include "lox_clock.h"

#ifdef _WIN32
#include <windows.h>

static uint64_t lox_clock_windows_now(void)
{
    LARGE_INTEGER value;
    QueryPerformanceCounter(&value);
    return (uint64_t)value.QuadPart;
}

const lox_clock_tick_source_t *lox_clock_default(void)
{
    static lox_clock_tick_source_t clock;
    static int initialized;

    if (!initialized) {
        LARGE_INTEGER frequency;
        clock.now_ticks = lox_clock_windows_now;
        clock.ticks_per_second = QueryPerformanceFrequency(&frequency) ? (uint64_t)frequency.QuadPart : 1000000ull;
        initialized = 1;
    }
    return &clock;
}

#endif
