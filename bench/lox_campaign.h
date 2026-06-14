#ifndef LOXSORT_BENCH_LOX_CAMPAIGN_H
#define LOXSORT_BENCH_LOX_CAMPAIGN_H

#include "lox_bench.h"
#include "lox_generators.h"

#include <stddef.h>
#include <stdint.h>

typedef enum lox_campaign_family_e {
    LOX_CAMPAIGN_FAMILY_INTEGERS = 0,
    LOX_CAMPAIGN_FAMILY_FLOATING_POINT,
    LOX_CAMPAIGN_FAMILY_CHARACTERS,
    LOX_CAMPAIGN_FAMILY_STRINGS,
    LOX_CAMPAIGN_FAMILY_RECORDS,
    LOX_CAMPAIGN_FAMILY_TIMESTAMPS
} lox_campaign_family_t;

typedef enum lox_campaign_data_type_e {
    LOX_CAMPAIGN_TYPE_INT8 = 0,
    LOX_CAMPAIGN_TYPE_UINT8,
    LOX_CAMPAIGN_TYPE_INT16,
    LOX_CAMPAIGN_TYPE_UINT16,
    LOX_CAMPAIGN_TYPE_INT32,
    LOX_CAMPAIGN_TYPE_UINT32,
    LOX_CAMPAIGN_TYPE_INT64,
    LOX_CAMPAIGN_TYPE_UINT64,
    LOX_CAMPAIGN_TYPE_FLOAT,
    LOX_CAMPAIGN_TYPE_DOUBLE,
    LOX_CAMPAIGN_TYPE_RAW_BYTES,
    LOX_CAMPAIGN_TYPE_ASCII,
    LOX_CAMPAIGN_TYPE_DIGITS,
    LOX_CAMPAIGN_TYPE_LOWERCASE,
    LOX_CAMPAIGN_TYPE_UPPERCASE,
    LOX_CAMPAIGN_TYPE_ALNUM,
    LOX_CAMPAIGN_TYPE_CASE_INSENSITIVE_ASCII,
    LOX_CAMPAIGN_TYPE_FIXED_STRING_8,
    LOX_CAMPAIGN_TYPE_FIXED_STRING_16,
    LOX_CAMPAIGN_TYPE_FIXED_STRING_32,
    LOX_CAMPAIGN_TYPE_FIXED_STRING_64,
    LOX_CAMPAIGN_TYPE_FIXED_STRING_128,
    LOX_CAMPAIGN_TYPE_POINTER_STRING,
    LOX_CAMPAIGN_TYPE_SENSOR_RECORD,
    LOX_CAMPAIGN_TYPE_PACKET_RECORD,
    LOX_CAMPAIGN_TYPE_LOG_RECORD,
    LOX_CAMPAIGN_TYPE_SCHEDULER_TASK,
    LOX_CAMPAIGN_TYPE_DATABASE_RECORD,
    LOX_CAMPAIGN_TYPE_DEVICE_EVENT,
    LOX_CAMPAIGN_TYPE_TICKS32,
    LOX_CAMPAIGN_TYPE_TICKS64,
    LOX_CAMPAIGN_TYPE_TIMESTAMP_SEQUENCE,
    LOX_CAMPAIGN_TYPE_TIMESTAMP_DEVICE,
    LOX_CAMPAIGN_TYPE_TIMESTAMP_PRIORITY
} lox_campaign_data_type_t;

typedef enum lox_campaign_comparator_kind_e {
    LOX_CAMPAIGN_CMP_NUMERIC = 0,
    LOX_CAMPAIGN_CMP_UNSIGNED,
    LOX_CAMPAIGN_CMP_TOTAL_ORDER,
    LOX_CAMPAIGN_CMP_CASE_SENSITIVE,
    LOX_CAMPAIGN_CMP_CASE_INSENSITIVE,
    LOX_CAMPAIGN_CMP_PRIORITY,
    LOX_CAMPAIGN_CMP_PRIORITY_TIMESTAMP_ID,
    LOX_CAMPAIGN_CMP_ID,
    LOX_CAMPAIGN_CMP_TIMESTAMP,
    LOX_CAMPAIGN_CMP_SEQUENCE,
    LOX_CAMPAIGN_CMP_DEVICE,
    LOX_CAMPAIGN_CMP_TEXT_FIELD
} lox_campaign_comparator_kind_t;

typedef enum lox_campaign_scratch_mode_e {
    LOX_CAMPAIGN_SCRATCH_NO = 0,
    LOX_CAMPAIGN_SCRATCH_EXACT,
    LOX_CAMPAIGN_SCRATCH_AMPLE,
    LOX_CAMPAIGN_SCRATCH_INSUFFICIENT
} lox_campaign_scratch_mode_t;

typedef enum lox_campaign_strategy_e {
    LOX_CAMPAIGN_STRATEGY_ALWAYS_INSERTION = 0,
    LOX_CAMPAIGN_STRATEGY_ALWAYS_SHELL,
    LOX_CAMPAIGN_STRATEGY_ALWAYS_INTRO,
    LOX_CAMPAIGN_STRATEGY_ALWAYS_MERGE_IF_AVAILABLE,
    LOX_CAMPAIGN_STRATEGY_ALWAYS_CYCLE_IF_AVAILABLE,
    LOX_CAMPAIGN_STRATEGY_LOXSORT_DISPATCHER,
    LOX_CAMPAIGN_STRATEGY_ORACLE_BEST
} lox_campaign_strategy_t;

typedef enum lox_campaign_size_e {
    LOX_CAMPAIGN_SMOKE = 0,
    LOX_CAMPAIGN_MEDIUM,
    LOX_CAMPAIGN_FULL_1M
} lox_campaign_size_t;

typedef struct lox_campaign_plan_s {
    lox_campaign_size_t size;
    size_t dataset_count;
    uint64_t seed_base;
} lox_campaign_plan_t;

typedef struct lox_campaign_dataset_s {
    size_t dataset_id;
    lox_campaign_family_t data_family;
    lox_campaign_data_type_t data_type;
    lox_bench_pattern_t pattern;
    size_t count;
    size_t element_size;
    lox_campaign_comparator_kind_t comparator_kind;
    uint64_t seed;
    lox_campaign_scratch_mode_t scratch_mode;
    char sample_preview[256];
} lox_campaign_dataset_t;

typedef struct lox_campaign_strategy_result_s {
    lox_campaign_strategy_t strategy;
    lox_algorithm_t selected_algorithm;
    lox_algorithm_t best_algorithm;
    lox_decision_reason_t decision_reason;
    uint8_t profile_fallback_used;
    uint8_t sampled_pair_count;
    uint8_t disorder_score;
    uint8_t equal_score;
    uint8_t equal_pair_count;
    uint8_t direction_changes;
    uint8_t comparison_sign_mask;
    uint64_t clock_frequency_hz;
    uint64_t timing_iterations;
    uint64_t sample_count;
    uint64_t elapsed_ticks_total;
    uint64_t ticks_per_iteration;
    double min_ns;
    double median_ns;
    double mean_ns;
    double p95_ns;
    double max_ns;
    double feature_extraction_ns;
    double dispatch_only_ns;
    double selected_sort_ns;
    double public_api_total_ns;
    double dispatcher_overhead_ns;
    double dispatcher_overhead_percent;
    uint64_t comparison_count;
    uint64_t move_count;
    uint64_t swap_count;
    uint64_t bytes_moved;
    int sorted_ok;
    int multiset_ok;
    int payload_ok;
    int canary_ok;
    double regret;
    double absolute_loss_ns;
} lox_campaign_strategy_result_t;

int lox_campaign_run_cli(int argc, char **argv);
const char *lox_campaign_family_name(lox_campaign_family_t family);
const char *lox_campaign_data_type_name(lox_campaign_data_type_t type);
const char *lox_campaign_comparator_name(lox_campaign_comparator_kind_t kind);
const char *lox_campaign_scratch_mode_name(lox_campaign_scratch_mode_t mode);
const char *lox_campaign_strategy_name(lox_campaign_strategy_t strategy);
size_t lox_campaign_plan_dataset_count(lox_campaign_size_t size);
int lox_campaign_parse_size(const char *text, lox_campaign_size_t *size);

#endif
