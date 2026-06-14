#ifndef LOXSORT_BENCH_LOX_BENCH_H
#define LOXSORT_BENCH_LOX_BENCH_H

#include "loxsort/loxsort.h"
#include <stddef.h>

typedef enum lox_bench_pattern_e {
    LOX_BENCH_PATTERN_RANDOM = 0,
    LOX_BENCH_PATTERN_SORTED,
    LOX_BENCH_PATTERN_REVERSE,
    LOX_BENCH_PATTERN_NEARLY_SORTED,
    LOX_BENCH_PATTERN_FEW_UNIQUE_2,
    LOX_BENCH_PATTERN_FEW_UNIQUE_8,
    LOX_BENCH_PATTERN_ALL_EQUAL,
    LOX_BENCH_PATTERN_ORGAN_PIPE,
    LOX_BENCH_PATTERN_SAWTOOTH,
    LOX_BENCH_PATTERN_SHUFFLED_RUNS,
    LOX_BENCH_PATTERN_ADVERSARIAL
} lox_bench_pattern_t;

typedef struct lox_bench_result_s {
    lox_status_t status;
    lox_sort_result_t sort_result;
    uint64_t elapsed_ticks;
} lox_bench_result_t;

void lox_bench_run_demo(void);
const char *lox_bench_pattern_name(lox_bench_pattern_t pattern);

#endif
