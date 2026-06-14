#include "lox_clock.h"

#ifdef _WIN32
#include <windows.h>

static uint64_t lox_clock_windows_now(void)
{
    LARGE_INTEGER value;
    QueryPerformanceCounter(&value);
    return (uint64_t)value.QuadPart;
}

static const lox_clock_tick_source_t g_clock = { lox_clock_windows_now, 0u };

const lox_clock_tick_source_t *lox_clock_default(void)
{
    return &g_clock;
}

#endif

