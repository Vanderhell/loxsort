#ifndef LOXSORT_BENCH_LOX_CLOCK_H
#define LOXSORT_BENCH_LOX_CLOCK_H

#include <stdint.h>

typedef struct lox_clock_tick_source_s {
    uint64_t (*now_ticks)(void);
    uint64_t ticks_per_second;
} lox_clock_tick_source_t;

const lox_clock_tick_source_t *lox_clock_default(void);

#endif

