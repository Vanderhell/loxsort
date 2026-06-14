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
    LOX_BENCH_PATTERN_STAGGER,
    LOX_BENCH_PATTERN_PLATEAU,
    LOX_BENCH_PATTERN_SAWTOOTH,
    LOX_BENCH_PATTERN_DESCENDING_BLOCKS_4,
    LOX_BENCH_PATTERN_REVERSE_FIRST_HALF,
    LOX_BENCH_PATTERN_REVERSE_SECOND_HALF,
    LOX_BENCH_PATTERN_RANDOM_HALF,
    LOX_BENCH_PATTERN_RANDOM_TAIL,
    LOX_BENCH_PATTERN_PUSH_FRONT,
    LOX_BENCH_PATTERN_PUSH_MIDDLE,
    LOX_BENCH_PATTERN_ASCENDING_RUNS,
    LOX_BENCH_PATTERN_DESCENDING_RUNS,
    LOX_BENCH_PATTERN_ALTERNATING_HIGH_LOW,
    LOX_BENCH_PATTERN_BIT_REVERSAL,
    LOX_BENCH_PATTERN_ADVERSARIAL
} lox_bench_pattern_t;

typedef enum lox_bench_split_e {
    LOX_BENCH_SPLIT_TRAIN = 0,
    LOX_BENCH_SPLIT_VALIDATION,
    LOX_BENCH_SPLIT_TEST
} lox_bench_split_t;

typedef struct lox_bench_result_s {
    lox_status_t status;
    lox_sort_result_t sort_result;
    uint64_t elapsed_ticks;
} lox_bench_result_t;

void lox_bench_run_demo(void);
const char *lox_bench_pattern_name(lox_bench_pattern_t pattern);
const char *lox_bench_split_name(lox_bench_split_t split);

#endif
