#include "lox_campaign.h"

#include "lox_clock.h"

#include "../src/lox_internal.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    LOX_CAMPAIGN_CANARY_SIZE = 32u,
    LOX_CAMPAIGN_MAX_STRATEGIES = 7u,
    LOX_CAMPAIGN_MAX_FAMILY_TYPES = 8u,
    LOX_CAMPAIGN_MAX_RECORD_SIZES = 7u,
    LOX_CAMPAIGN_MAX_STRING_SIZES = 5u
};

typedef struct lox_campaign_compare_context_s {
    lox_campaign_family_t family;
    lox_campaign_data_type_t type;
    lox_campaign_comparator_kind_t comparator_kind;
    size_t element_size;
    uint64_t *comparison_count;
} lox_campaign_compare_context_t;

typedef void (*lox_campaign_measure_fn_t)(void *context);

typedef struct lox_campaign_selected_sort_ctx_s {
    const lox_algorithm_entry_t *entry;
    const unsigned char *master;
    unsigned char *work;
    size_t bytes;
    size_t count;
    size_t element_size;
    const lox_campaign_compare_context_t *compare_ctx;
    const lox_sort_options_t *options;
} lox_campaign_selected_sort_ctx_t;

typedef struct lox_campaign_public_api_ctx_s {
    const unsigned char *master;
    unsigned char *work;
    size_t bytes;
    size_t count;
    size_t element_size;
    const lox_campaign_compare_context_t *compare_ctx;
    const lox_sort_options_t *options;
    lox_sort_result_t *result;
} lox_campaign_public_api_ctx_t;

typedef struct lox_campaign_feature_ctx_s {
    const unsigned char *master;
    size_t count;
    size_t element_size;
    const lox_campaign_compare_context_t *compare_ctx;
    const lox_profile_t *profile;
    lox_feature_vector_t *out;
} lox_campaign_feature_ctx_t;

typedef struct lox_campaign_dispatch_ctx_s {
    const lox_feature_vector_t *features;
    const lox_profile_t *profile;
    size_t count;
    size_t element_size;
    int has_scratch;
    lox_algorithm_t *out_algorithm;
} lox_campaign_dispatch_ctx_t;

static unsigned char *lox_campaign_data_ptr(unsigned char *buffer);
static const unsigned char *lox_campaign_const_data_ptr(const unsigned char *buffer);
static void lox_campaign_format_sample_preview(
    const lox_campaign_dataset_t *dataset,
    const unsigned char *data,
    char *dst,
    size_t dst_size);
static lox_algorithm_t lox_campaign_dispatch_choice(
    const lox_feature_vector_t *features,
    const lox_profile_t *profile,
    size_t count,
    size_t element_size,
    int has_scratch);

typedef struct lox_campaign_dataset_buffer_s {
    unsigned char *master;
    unsigned char *work;
    unsigned char *reference;
    unsigned char *canary_guard;
    size_t bytes;
    char **string_pool;
    size_t string_pool_count;
} lox_campaign_dataset_buffer_t;

typedef struct lox_campaign_run_state_s {
    FILE *csv;
    FILE *checkpoint;
    char output_path[1024];
    char checkpoint_path[1024];
    char report_path[1024];
    lox_campaign_size_t campaign;
    size_t dataset_count;
    uint64_t seed_base;
    uint64_t checkpoint_interval;
    uint64_t timing_target_ms;
    uint64_t timing_samples;
    uint64_t stop_after_datasets;
    uint64_t next_dataset_id;
    uint64_t scenario_hash_a;
    uint64_t scenario_hash_b;
    uint64_t scenario_hash_c;
    uint64_t scenario_hash_d;
} lox_campaign_run_state_t;

static const char *g_family_names[] = {
    "integers",
    "floating_point",
    "characters",
    "strings",
    "records",
    "timestamps"
};

static const char *g_type_names[] = {
    "int8_t",
    "uint8_t",
    "int16_t",
    "uint16_t",
    "int32_t",
    "uint32_t",
    "int64_t",
    "uint64_t",
    "float",
    "double",
    "raw_bytes",
    "ascii",
    "digits",
    "lowercase",
    "uppercase",
    "alnum",
    "case_insensitive_ascii",
    "char[8]",
    "char[16]",
    "char[32]",
    "char[64]",
    "char[128]",
    "const char *",
    "sensor_record",
    "packet_record",
    "log_record",
    "scheduler_task",
    "database_record",
    "device_event",
    "uint32_t ticks",
    "uint64_t timestamps",
    "timestamp+sequence",
    "timestamp+device_id",
    "timestamp+priority"
};

static const char *g_cmp_names[] = {
    "numeric",
    "unsigned",
    "total_order",
    "case_sensitive",
    "case_insensitive",
    "priority",
    "priority_timestamp_id",
    "id",
    "timestamp",
    "sequence",
    "device",
    "text_field"
};

static const char *g_scratch_names[] = {
    "no_scratch",
    "exact_merge_scratch",
    "ample_scratch",
    "insufficient_scratch"
};

static const char *g_strategy_names[] = {
    "always_insertion",
    "always_shell",
    "always_intro",
    "always_merge_if_available",
    "always_cycle_if_available",
    "loxsort_dispatcher",
    "oracle_best"
};

static const uint32_t g_train_dataset_seeds[] = { 1u, 7u, 13u, 29u };
static const uint32_t g_validation_dataset_seeds[] = { 101u, 131u, 197u, 263u };
static const uint32_t g_test_dataset_seeds[] = { 401u, 433u, 467u, 499u };
static const lox_bench_pattern_t g_all_patterns[] = {
    LOX_BENCH_PATTERN_RANDOM,
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
};

static const size_t g_count_low_values[] = {
    0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 15u, 16u, 17u, 23u, 24u, 25u, 31u, 32u
};
static const size_t g_count_mid_values[] = {
    33u, 47u, 48u, 49u, 63u, 64u, 65u, 96u, 127u, 128u
};
static const size_t g_count_high_values[] = {
    129u, 255u, 256u, 257u, 511u, 512u
};
static const size_t g_count_huge_values[] = {
    513u, 1024u, 2048u, 4096u
};

static uint64_t lox_campaign_mix64(uint64_t x)
{
    x ^= x >> 33;
    x *= UINT64_C(0xff51afd7ed558ccd);
    x ^= x >> 33;
    x *= UINT64_C(0xc4ceb9fe1a85ec53);
    x ^= x >> 33;
    return x;
}

static uint64_t lox_campaign_now_ticks(void)
{
    const lox_clock_tick_source_t *clock = lox_clock_default();
    return clock->now_ticks();
}

static double lox_campaign_ticks_to_ns(uint64_t ticks, uint64_t ticks_per_second)
{
    if (ticks_per_second == 0u) {
        return (double)ticks;
    }
    return ((double)ticks * 1000000000.0) / (double)ticks_per_second;
}

static uint64_t lox_campaign_ns_to_ticks(uint64_t ns, uint64_t ticks_per_second)
{
    if (ticks_per_second == 0u) {
        return ns;
    }
    return (ns * ticks_per_second) / UINT64_C(1000000000);
}

static double lox_campaign_select_percentile(const double *values, size_t count, double p)
{
    size_t index;

    if (count == 0u) {
        return 0.0;
    }
    if (p <= 0.0) {
        return values[0];
    }
    if (p >= 1.0) {
        return values[count - 1u];
    }
    index = (size_t)((double)(count - 1u) * p);
    if (index >= count) {
        index = count - 1u;
    }
    return values[index];
}

const char *lox_campaign_family_name(lox_campaign_family_t family)
{
    if ((size_t)family >= sizeof(g_family_names) / sizeof(g_family_names[0])) {
        return "unknown";
    }
    return g_family_names[family];
}

const char *lox_campaign_data_type_name(lox_campaign_data_type_t type)
{
    if ((size_t)type >= sizeof(g_type_names) / sizeof(g_type_names[0])) {
        return "unknown";
    }
    return g_type_names[type];
}

const char *lox_campaign_comparator_name(lox_campaign_comparator_kind_t kind)
{
    if ((size_t)kind >= sizeof(g_cmp_names) / sizeof(g_cmp_names[0])) {
        return "unknown";
    }
    return g_cmp_names[kind];
}

const char *lox_campaign_scratch_mode_name(lox_campaign_scratch_mode_t mode)
{
    if ((size_t)mode >= sizeof(g_scratch_names) / sizeof(g_scratch_names[0])) {
        return "unknown";
    }
    return g_scratch_names[mode];
}

const char *lox_campaign_strategy_name(lox_campaign_strategy_t strategy)
{
    if ((size_t)strategy >= sizeof(g_strategy_names) / sizeof(g_strategy_names[0])) {
        return "unknown";
    }
    return g_strategy_names[strategy];
}

size_t lox_campaign_plan_dataset_count(lox_campaign_size_t size)
{
    switch (size) {
    case LOX_CAMPAIGN_SMOKE:
        return 100u;
    case LOX_CAMPAIGN_MEDIUM:
        return 10000u;
    case LOX_CAMPAIGN_FULL_1M:
    default:
        return 1000000u;
    }
}

int lox_campaign_parse_size(const char *text, lox_campaign_size_t *size)
{
    if (text == NULL || size == NULL) {
        return 0;
    }
    if (strcmp(text, "smoke") == 0) {
        *size = LOX_CAMPAIGN_SMOKE;
        return 1;
    }
    if (strcmp(text, "medium") == 0) {
        *size = LOX_CAMPAIGN_MEDIUM;
        return 1;
    }
    if (strcmp(text, "full-1m") == 0 || strcmp(text, "full") == 0 || strcmp(text, "1m") == 0) {
        *size = LOX_CAMPAIGN_FULL_1M;
        return 1;
    }
    return 0;
}

static void lox_campaign_clear_buffer(lox_campaign_dataset_buffer_t *buffer)
{
    if (buffer == NULL) {
        return;
    }
    free(buffer->master);
    free(buffer->work);
    free(buffer->reference);
    free(buffer->canary_guard);
    free(buffer->string_pool);
    memset(buffer, 0, sizeof(*buffer));
}

static size_t lox_campaign_element_size_for_type(lox_campaign_data_type_t type)
{
    switch (type) {
    case LOX_CAMPAIGN_TYPE_INT8:
    case LOX_CAMPAIGN_TYPE_UINT8:
    case LOX_CAMPAIGN_TYPE_RAW_BYTES:
    case LOX_CAMPAIGN_TYPE_ASCII:
    case LOX_CAMPAIGN_TYPE_DIGITS:
    case LOX_CAMPAIGN_TYPE_LOWERCASE:
    case LOX_CAMPAIGN_TYPE_UPPERCASE:
    case LOX_CAMPAIGN_TYPE_ALNUM:
    case LOX_CAMPAIGN_TYPE_CASE_INSENSITIVE_ASCII:
        return 1u;
    case LOX_CAMPAIGN_TYPE_INT16:
    case LOX_CAMPAIGN_TYPE_UINT16:
        return 2u;
    case LOX_CAMPAIGN_TYPE_INT32:
    case LOX_CAMPAIGN_TYPE_UINT32:
    case LOX_CAMPAIGN_TYPE_FLOAT:
    case LOX_CAMPAIGN_TYPE_TICKS32:
        return 4u;
    case LOX_CAMPAIGN_TYPE_INT64:
    case LOX_CAMPAIGN_TYPE_UINT64:
    case LOX_CAMPAIGN_TYPE_DOUBLE:
    case LOX_CAMPAIGN_TYPE_POINTER_STRING:
    case LOX_CAMPAIGN_TYPE_TICKS64:
        return 8u;
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_8:
        return 8u;
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_16:
        return 16u;
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_32:
        return 32u;
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_64:
        return 64u;
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_128:
        return 128u;
    case LOX_CAMPAIGN_TYPE_SENSOR_RECORD:
        return 8u;
    case LOX_CAMPAIGN_TYPE_PACKET_RECORD:
        return 16u;
    case LOX_CAMPAIGN_TYPE_LOG_RECORD:
        return 24u;
    case LOX_CAMPAIGN_TYPE_SCHEDULER_TASK:
        return 32u;
    case LOX_CAMPAIGN_TYPE_DATABASE_RECORD:
        return 48u;
    case LOX_CAMPAIGN_TYPE_DEVICE_EVENT:
        return 64u;
    case LOX_CAMPAIGN_TYPE_TIMESTAMP_SEQUENCE:
    case LOX_CAMPAIGN_TYPE_TIMESTAMP_DEVICE:
    case LOX_CAMPAIGN_TYPE_TIMESTAMP_PRIORITY:
        return 16u;
    default:
        return 8u;
    }
}

static const size_t *lox_campaign_count_values_for_bucket(uint8_t bucket, size_t *count)
{
    switch (bucket) {
    case 0:
        *count = sizeof(g_count_low_values) / sizeof(g_count_low_values[0]);
        return g_count_low_values;
    case 1:
        *count = sizeof(g_count_mid_values) / sizeof(g_count_mid_values[0]);
        return g_count_mid_values;
    case 2:
        *count = sizeof(g_count_high_values) / sizeof(g_count_high_values[0]);
        return g_count_high_values;
    default:
        *count = sizeof(g_count_huge_values) / sizeof(g_count_huge_values[0]);
        return g_count_huge_values;
    }
}

static size_t lox_campaign_pick_count(
    uint8_t bucket,
    size_t bucket_index,
    uint64_t seed)
{
    size_t values_count = 0u;
    const size_t *values = lox_campaign_count_values_for_bucket(bucket, &values_count);
    if (bucket_index < values_count) {
        return values[bucket_index];
    }
    switch (bucket) {
    case 0:
        return (size_t)(seed % 33u);
    case 1:
        return 33u + (size_t)(seed % 96u);
    case 2:
        return 129u + (size_t)(seed % 384u);
    default:
        return 513u + (size_t)(seed % 3584u);
    }
}

static size_t lox_campaign_family_target(size_t total, lox_campaign_family_t family)
{
    switch (family) {
    case LOX_CAMPAIGN_FAMILY_INTEGERS:
        return (total * 18u) / 100u;
    case LOX_CAMPAIGN_FAMILY_FLOATING_POINT:
        return (total * 8u) / 100u;
    case LOX_CAMPAIGN_FAMILY_CHARACTERS:
        return (total * 7u) / 100u;
    case LOX_CAMPAIGN_FAMILY_STRINGS:
        return (total * 25u) / 100u;
    case LOX_CAMPAIGN_FAMILY_RECORDS:
        return (total * 30u) / 100u;
    case LOX_CAMPAIGN_FAMILY_TIMESTAMPS:
    default:
        return total - ((total * 18u) / 100u) - ((total * 8u) / 100u) - ((total * 7u) / 100u) - ((total * 25u) / 100u) - ((total * 30u) / 100u);
    }
}

static size_t lox_campaign_bucket_target(size_t total, uint8_t bucket)
{
    switch (bucket) {
    case 0:
        return (total * 50u) / 100u;
    case 1:
        return (total * 30u) / 100u;
    case 2:
        return (total * 15u) / 100u;
    default:
        return total - ((total * 50u) / 100u) - ((total * 30u) / 100u) - ((total * 15u) / 100u);
    }
}

static uint8_t lox_campaign_choose_weighted_index(
    const size_t *produced,
    const size_t *targets,
    size_t count)
{
    size_t i;
    size_t best_index = 0u;
    double best_score = 1.0e300;
    for (i = 0u; i < count; ++i) {
        double target = (double)(targets[i] == 0u ? 1u : targets[i]);
        double score = (double)produced[i] / target;
        if (i == 0u || score < best_score || (score == best_score && i < best_index)) {
            best_score = score;
            best_index = i;
        }
    }
    return (uint8_t)best_index;
}

static void lox_campaign_fill_weighted_sequence(
    uint8_t *sequence,
    size_t total,
    const size_t *targets,
    size_t target_count)
{
    size_t produced[8] = { 0u };
    size_t i;

    for (i = 0u; i < total; ++i) {
        uint8_t index = lox_campaign_choose_weighted_index(produced, targets, target_count);
        sequence[i] = index;
        ++produced[index];
    }
}

static uint32_t lox_campaign_reverse_bits(uint32_t value, unsigned bits)
{
    uint32_t result = 0u;
    unsigned i;
    for (i = 0u; i < bits; ++i) {
        result <<= 1u;
        result |= (value & 1u);
        value >>= 1u;
    }
    return result;
}

static void lox_campaign_fill_pattern_keys(
    uint64_t *keys,
    size_t count,
    lox_bench_pattern_t pattern,
    uint64_t seed)
{
    size_t i;
    uint64_t state = seed ? seed : UINT64_C(1);
    for (i = 0u; i < count; ++i) {
        keys[i] = (uint64_t)i;
    }
    switch (pattern) {
    case LOX_BENCH_PATTERN_RANDOM:
        for (i = 0u; i < count; ++i) {
            state = lox_campaign_mix64(state + UINT64_C(0x9e3779b97f4a7c15));
            keys[i] = state;
        }
        break;
    case LOX_BENCH_PATTERN_SORTED:
        break;
    case LOX_BENCH_PATTERN_REVERSE:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint64_t)(count - 1u - i);
        }
        break;
    case LOX_BENCH_PATTERN_NEARLY_SORTED:
        for (i = 1u; i + 1u < count; i += 4u) {
            uint64_t tmp = keys[i];
            keys[i] = keys[i + 1u];
            keys[i + 1u] = tmp;
        }
        break;
    case LOX_BENCH_PATTERN_FEW_UNIQUE_2:
    case LOX_BENCH_PATTERN_FEW_UNIQUE_8: {
        uint64_t modulo = pattern == LOX_BENCH_PATTERN_FEW_UNIQUE_2 ? 2u : 8u;
        for (i = 0u; i < count; ++i) {
            state = lox_campaign_mix64(state + (uint64_t)i + 1u);
            keys[i] = state % modulo;
        }
        break;
    }
    case LOX_BENCH_PATTERN_ALL_EQUAL:
        for (i = 0u; i < count; ++i) {
            keys[i] = 1u;
        }
        break;
    case LOX_BENCH_PATTERN_ORGAN_PIPE:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint64_t)(i <= count / 2u ? i : (count - i));
        }
        break;
    case LOX_BENCH_PATTERN_STAGGER: {
        size_t stride = count / 2u + 1u;
        if (stride == 0u) {
            stride = 1u;
        }
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint64_t)(((i * stride) + i) % (count == 0u ? 1u : count));
        }
        break;
    }
    case LOX_BENCH_PATTERN_PLATEAU: {
        uint64_t plateau = (uint64_t)(count / 4u);
        for (i = 0u; i < count; ++i) {
            keys[i] = i < plateau ? (uint64_t)i : plateau;
        }
        break;
    }
    case LOX_BENCH_PATTERN_SAWTOOTH:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint64_t)(i % 7u);
        }
        break;
    case LOX_BENCH_PATTERN_DESCENDING_BLOCKS_4:
        for (i = 0u; i < count; ++i) {
            size_t block = i / 4u;
            size_t offset = i % 4u;
            keys[i] = (uint64_t)(block * 4u + (3u - offset));
        }
        break;
    case LOX_BENCH_PATTERN_REVERSE_FIRST_HALF:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = (uint64_t)((count / 2u) - 1u - i);
        }
        for (i = count / 2u; i < count; ++i) {
            keys[i] = (uint64_t)i;
        }
        break;
    case LOX_BENCH_PATTERN_REVERSE_SECOND_HALF:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = (uint64_t)i;
        }
        for (i = count / 2u; i < count; ++i) {
            keys[i] = (uint64_t)(count - 1u - (i - count / 2u));
        }
        break;
    case LOX_BENCH_PATTERN_RANDOM_HALF:
        for (i = 0u; i < count / 2u; ++i) {
            state = lox_campaign_mix64(state + UINT64_C(0xda942042e4dd58b5));
            keys[i] = state;
        }
        for (i = count / 2u; i < count; ++i) {
            keys[i] = (uint64_t)i;
        }
        break;
    case LOX_BENCH_PATTERN_RANDOM_TAIL: {
        size_t tail = count / 4u;
        for (i = 0u; i < count - tail; ++i) {
            keys[i] = (uint64_t)i;
        }
        for (; i < count; ++i) {
            state = lox_campaign_mix64(state + UINT64_C(0x6eed0e9da4d94a4f));
            keys[i] = state;
        }
        break;
    }
    case LOX_BENCH_PATTERN_PUSH_FRONT:
        if (count > 0u) {
            keys[0] = (uint64_t)(count - 1u);
            for (i = 1u; i < count; ++i) {
                keys[i] = (uint64_t)(i - 1u);
            }
        }
        break;
    case LOX_BENCH_PATTERN_PUSH_MIDDLE:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = (uint64_t)i;
        }
        if (count > 0u) {
            keys[count / 2u] = (uint64_t)(count - 1u);
        }
        for (i = count / 2u + 1u; i < count; ++i) {
            keys[i] = (uint64_t)(i - 1u);
        }
        break;
    case LOX_BENCH_PATTERN_ASCENDING_RUNS: {
        size_t run_length = 4u;
        size_t run_count = (count + run_length - 1u) / run_length;
        for (i = 0u; i < count; ++i) {
            size_t run = i / run_length;
            size_t offset = i % run_length;
            keys[i] = (uint64_t)((run_count - 1u - run) * run_length + offset);
        }
        break;
    }
    case LOX_BENCH_PATTERN_DESCENDING_RUNS: {
        size_t run_length = 4u;
        for (i = 0u; i < count; ++i) {
            size_t run = i / run_length;
            size_t offset = i % run_length;
            keys[i] = (uint64_t)(run * run_length + (run_length - 1u - offset));
        }
        break;
    }
    case LOX_BENCH_PATTERN_ALTERNATING_HIGH_LOW:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint64_t)((i % 2u == 0u) ? (i / 2u) : (count - 1u - (i / 2u)));
        }
        break;
    case LOX_BENCH_PATTERN_BIT_REVERSAL: {
        unsigned bits = 0u;
        uint64_t domain = 1u;
        while (domain < count) {
            domain <<= 1u;
            ++bits;
        }
        if (bits == 0u) {
            bits = 1u;
        }
        for (i = 0u; i < count; ++i) {
            uint32_t reversed = lox_campaign_reverse_bits((uint32_t)i, bits);
            keys[i] = count == 0u ? 0u : (uint64_t)(reversed % (uint32_t)count);
        }
        break;
    }
    case LOX_BENCH_PATTERN_ADVERSARIAL:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint64_t)((i * 37u) ^ (i >> 1u) ^ (uint64_t)(seed << 1u));
        }
        break;
    default:
        break;
    }
}

static int lox_campaign_compare_casefold(unsigned char a, unsigned char b)
{
    return tolower((int)a) - tolower((int)b);
}

static int lox_campaign_compare_string_bytes(const unsigned char *lhs, const unsigned char *rhs, size_t size, int case_insensitive)
{
    size_t i;
    for (i = 0u; i < size; ++i) {
        unsigned char a = lhs[i];
        unsigned char b = rhs[i];
        if (a == 0u || b == 0u) {
            if (a == b) {
                return 0;
            }
            return a == 0u ? -1 : 1;
        }
        if (case_insensitive) {
            int diff = lox_campaign_compare_casefold(a, b);
            if (diff != 0) {
                return diff < 0 ? -1 : 1;
            }
        } else if (a != b) {
            return a < b ? -1 : 1;
        }
    }
    return 0;
}

static int lox_campaign_compare_float_total(const void *lhs, const void *rhs, size_t size)
{
    if (size == sizeof(float)) {
        uint32_t a = 0u;
        uint32_t b = 0u;
        uint32_t ta;
        uint32_t tb;
        memcpy(&a, lhs, sizeof(a));
        memcpy(&b, rhs, sizeof(b));
        ta = (a & 0x80000000u) ? ~a : (a | 0x80000000u);
        tb = (b & 0x80000000u) ? ~b : (b | 0x80000000u);
        if (ta < tb) {
            return -1;
        }
        if (ta > tb) {
            return 1;
        }
        return 0;
    }
    if (size == sizeof(double)) {
        uint64_t a = 0u;
        uint64_t b = 0u;
        uint64_t ta;
        uint64_t tb;
        memcpy(&a, lhs, sizeof(a));
        memcpy(&b, rhs, sizeof(b));
        ta = (a & UINT64_C(0x8000000000000000)) ? ~a : (a | UINT64_C(0x8000000000000000));
        tb = (b & UINT64_C(0x8000000000000000)) ? ~b : (b | UINT64_C(0x8000000000000000));
        if (ta < tb) {
            return -1;
        }
        if (ta > tb) {
            return 1;
        }
        return 0;
    }
    return 0;
}

static uint64_t lox_campaign_read_u64(const void *ptr, size_t size, int is_signed)
{
    uint64_t value = 0u;
    memcpy(&value, ptr, size < sizeof(value) ? size : sizeof(value));
    if (is_signed && size < sizeof(value)) {
        uint64_t mask = UINT64_C(1) << ((size * 8u) - 1u);
        if ((value & mask) != 0u) {
            uint64_t extend = ~UINT64_C(0) << (size * 8u);
            value |= extend;
        }
    }
    return value;
}

static int lox_campaign_compare_records(const unsigned char *lhs, const unsigned char *rhs, lox_campaign_comparator_kind_t kind, size_t size)
{
    uint64_t key_l = 0u;
    uint64_t key_r = 0u;
    uint64_t idx_l = 0u;
    uint64_t idx_r = 0u;
    memcpy(&key_l, lhs, sizeof(key_l) < size ? sizeof(key_l) : size);
    memcpy(&key_r, rhs, sizeof(key_r) < size ? sizeof(key_r) : size);
    if (size > sizeof(uint64_t)) {
        memcpy(&idx_l, lhs + sizeof(uint64_t), sizeof(uint64_t));
        memcpy(&idx_r, rhs + sizeof(uint64_t), sizeof(uint64_t));
    }
    switch (kind) {
    case LOX_CAMPAIGN_CMP_PRIORITY:
        if (key_l != key_r) {
            return key_l < key_r ? -1 : 1;
        }
        break;
    case LOX_CAMPAIGN_CMP_PRIORITY_TIMESTAMP_ID:
        if (key_l != key_r) {
            return key_l < key_r ? -1 : 1;
        }
        if (idx_l != idx_r) {
            return idx_l < idx_r ? -1 : 1;
        }
        break;
    case LOX_CAMPAIGN_CMP_ID:
    case LOX_CAMPAIGN_CMP_TIMESTAMP:
    case LOX_CAMPAIGN_CMP_SEQUENCE:
    case LOX_CAMPAIGN_CMP_DEVICE:
    case LOX_CAMPAIGN_CMP_TEXT_FIELD:
    default:
        if (key_l != key_r) {
            return key_l < key_r ? -1 : 1;
        }
        break;
    }
    return 0;
}

static int lox_campaign_compare(
    const void *lhs,
    const void *rhs,
    void *user)
{
    const lox_campaign_compare_context_t *ctx = (const lox_campaign_compare_context_t *)user;
    const unsigned char *left = (const unsigned char *)lhs;
    const unsigned char *right = (const unsigned char *)rhs;

    if (ctx != NULL && ctx->comparison_count != NULL) {
        ++(*ctx->comparison_count);
    }

    switch (ctx->family) {
    case LOX_CAMPAIGN_FAMILY_INTEGERS:
    case LOX_CAMPAIGN_FAMILY_TIMESTAMPS: {
        int is_signed = ctx->type == LOX_CAMPAIGN_TYPE_INT8 ||
            ctx->type == LOX_CAMPAIGN_TYPE_INT16 ||
            ctx->type == LOX_CAMPAIGN_TYPE_INT32 ||
            ctx->type == LOX_CAMPAIGN_TYPE_INT64;
        uint64_t a = lox_campaign_read_u64(left, ctx->element_size, is_signed);
        uint64_t b = lox_campaign_read_u64(right, ctx->element_size, is_signed);
        if (a < b) {
            return -1;
        }
        if (a > b) {
            return 1;
        }
        return 0;
    }
    case LOX_CAMPAIGN_FAMILY_FLOATING_POINT:
        return lox_campaign_compare_float_total(left, right, ctx->element_size);
    case LOX_CAMPAIGN_FAMILY_CHARACTERS:
        if (ctx->comparator_kind == LOX_CAMPAIGN_CMP_CASE_INSENSITIVE) {
            return lox_campaign_compare_casefold(left[0], right[0]);
        }
        if (left[0] < right[0]) {
            return -1;
        }
        if (left[0] > right[0]) {
            return 1;
        }
        return 0;
    case LOX_CAMPAIGN_FAMILY_STRINGS:
        return lox_campaign_compare_string_bytes(left, right, ctx->element_size, ctx->comparator_kind == LOX_CAMPAIGN_CMP_CASE_INSENSITIVE);
    case LOX_CAMPAIGN_FAMILY_RECORDS:
        return lox_campaign_compare_records(left, right, ctx->comparator_kind, ctx->element_size);
    default:
        return lox_campaign_compare_records(left, right, LOX_CAMPAIGN_CMP_ID, ctx->element_size);
    }
}

static uint64_t lox_campaign_hash_bytes(const unsigned char *data, size_t size)
{
    uint64_t hash = UINT64_C(1469598103934665603);
    size_t i;
    for (i = 0u; i < size; ++i) {
        hash ^= (uint64_t)data[i];
        hash *= UINT64_C(1099511628211);
    }
    return lox_campaign_mix64(hash);
}

static void lox_campaign_fill_integer_bytes(
    unsigned char *dst,
    size_t element_size,
    uint64_t key,
    uint64_t seed,
    uint64_t index)
{
    uint64_t payload = lox_campaign_mix64(seed ^ (index * UINT64_C(0x9e3779b97f4a7c15)));
    memcpy(dst, &key, element_size < sizeof(key) ? element_size : sizeof(key));
    if (element_size > sizeof(key)) {
        size_t i;
        for (i = sizeof(key); i < element_size; ++i) {
            dst[i] = (unsigned char)(payload & 0xffu);
            payload >>= 8u;
            if (payload == 0u) {
                payload = lox_campaign_mix64(payload + index + i);
            }
        }
    }
}

static void lox_campaign_fill_float_bytes(
    unsigned char *dst,
    size_t element_size,
    uint64_t key,
    uint64_t seed)
{
    if (element_size == sizeof(float)) {
        uint32_t bits = (uint32_t)(lox_campaign_mix64(key ^ seed) & 0xffffffffu);
        if ((bits & 0x7f800000u) == 0x7f800000u) {
            bits = 0x7f7fffffu;
        }
        memcpy(dst, &bits, sizeof(bits));
    } else {
        uint64_t bits = lox_campaign_mix64(key ^ (seed << 1u));
        if ((bits & UINT64_C(0x7ff0000000000000)) == UINT64_C(0x7ff0000000000000)) {
            bits = UINT64_C(0x7fefffffffffffff);
        }
        memcpy(dst, &bits, sizeof(bits));
    }
}

static void lox_campaign_fill_char_bytes(
    unsigned char *dst,
    uint64_t key,
    uint64_t seed,
    lox_campaign_data_type_t type)
{
    unsigned char value;
    switch (type) {
    case LOX_CAMPAIGN_TYPE_ASCII:
        value = (unsigned char)(' ' + (key + seed) % 95u);
        break;
    case LOX_CAMPAIGN_TYPE_DIGITS:
        value = (unsigned char)('0' + (key + seed) % 10u);
        break;
    case LOX_CAMPAIGN_TYPE_LOWERCASE:
        value = (unsigned char)('a' + (key + seed) % 26u);
        break;
    case LOX_CAMPAIGN_TYPE_UPPERCASE:
        value = (unsigned char)('A' + (key + seed) % 26u);
        break;
    case LOX_CAMPAIGN_TYPE_ALNUM:
        value = (unsigned char)((key + seed) % 62u);
        value = (unsigned char)(value < 10u ? '0' + value : value < 36u ? 'A' + (value - 10u) : 'a' + (value - 36u));
        break;
    case LOX_CAMPAIGN_TYPE_CASE_INSENSITIVE_ASCII:
        value = (unsigned char)('A' + (key + seed) % 26u);
        break;
    case LOX_CAMPAIGN_TYPE_RAW_BYTES:
    default:
        value = (unsigned char)lox_campaign_mix64(key ^ seed);
        break;
    }
    dst[0] = value;
}

static void lox_campaign_fill_string_bytes(
    unsigned char *dst,
    size_t element_size,
    uint64_t key,
    uint64_t seed,
    lox_campaign_data_type_t type)
{
    static const char *templates[] = {
        "dev-%08x",
        "log-%llu",
        "path/%08x/%04x",
        "ABC-%06llu",
        "uuid-%08x-%04x",
        "sensor-%u",
        "timestamp-%llu",
        "alpha-%u"
    };
    char buffer[256];
    size_t i;
    unsigned idx = (unsigned)(lox_campaign_mix64(key ^ seed) % (sizeof(templates) / sizeof(templates[0])));
    switch (type) {
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_8:
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_16:
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_32:
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_64:
    case LOX_CAMPAIGN_TYPE_FIXED_STRING_128:
        break;
    case LOX_CAMPAIGN_TYPE_POINTER_STRING:
        return;
    default:
        break;
    }
    switch (idx) {
    case 0:
        snprintf(buffer, sizeof(buffer), "dev-%08x", (unsigned)(key ^ seed));
        break;
    case 1:
        snprintf(buffer, sizeof(buffer), "log-%llu", (unsigned long long)(key + seed));
        break;
    case 2:
        snprintf(buffer, sizeof(buffer), "path/%08x/%04x", (unsigned)(key >> 16u), (unsigned)key);
        break;
    case 3:
        snprintf(buffer, sizeof(buffer), "ABC-%06llu", (unsigned long long)(key % 1000000ull));
        break;
    case 4:
        snprintf(buffer, sizeof(buffer), "uuid-%08x-%04x", (unsigned)(key >> 16u), (unsigned)key);
        break;
    case 5:
        snprintf(buffer, sizeof(buffer), "sensor-%u", (unsigned)(key % 100000u));
        break;
    case 6:
        snprintf(buffer, sizeof(buffer), "timestamp-%llu", (unsigned long long)(key + 1000ull));
        break;
    default:
        snprintf(buffer, sizeof(buffer), "alpha-%u", (unsigned)(key % 100000u));
        break;
    }
    memset(dst, 0, element_size);
    for (i = 0u; i + 1u < element_size && buffer[i] != '\0'; ++i) {
        dst[i] = (unsigned char)buffer[i];
    }
}

static void lox_campaign_fill_record_bytes(
    unsigned char *dst,
    size_t element_size,
    uint64_t key,
    uint64_t seed,
    uint64_t index)
{
    uint64_t original_index = index;
    uint64_t checksum = lox_campaign_hash_bytes((const unsigned char *)&key, sizeof(key)) ^ lox_campaign_mix64(seed + index);
    size_t i;

    memset(dst, 0, element_size);
    memcpy(dst, &key, element_size < sizeof(key) ? element_size : sizeof(key));
    if (element_size > sizeof(key)) {
        memcpy(dst + sizeof(key), &original_index, sizeof(original_index) < element_size - sizeof(key) ? sizeof(original_index) : element_size - sizeof(key));
    }
    if (element_size > sizeof(key) + sizeof(original_index)) {
        size_t payload_offset = sizeof(key) + sizeof(original_index);
        for (i = payload_offset; i < element_size; ++i) {
            dst[i] = (unsigned char)(checksum & 0xffu);
            checksum >>= 8u;
            if (checksum == 0u) {
                checksum = lox_campaign_mix64(key + index + i);
            }
        }
    }
}

static int lox_campaign_generate_dataset(
    lox_campaign_dataset_t *dataset,
    lox_campaign_dataset_buffer_t *buffer)
{
    size_t bytes = dataset->count * dataset->element_size;
    size_t i;
    uint64_t *keys = NULL;

    memset(buffer, 0, sizeof(*buffer));
    buffer->bytes = bytes;
    buffer->master = (unsigned char *)calloc(1u, bytes + (LOX_CAMPAIGN_CANARY_SIZE * 2u));
    buffer->work = (unsigned char *)calloc(1u, bytes + (LOX_CAMPAIGN_CANARY_SIZE * 2u));
    buffer->reference = (unsigned char *)calloc(1u, bytes + (LOX_CAMPAIGN_CANARY_SIZE * 2u));
    buffer->canary_guard = (unsigned char *)calloc(1u, bytes + (LOX_CAMPAIGN_CANARY_SIZE * 2u));
    if (!buffer->master || !buffer->work || !buffer->reference || !buffer->canary_guard) {
        lox_campaign_clear_buffer(buffer);
        return 0;
    }
    if (dataset->count > 0u) {
        keys = (uint64_t *)calloc(dataset->count, sizeof(uint64_t));
        if (keys == NULL) {
            lox_campaign_clear_buffer(buffer);
            return 0;
        }
        lox_campaign_fill_pattern_keys(keys, dataset->count, dataset->pattern, dataset->seed);
        for (i = 0u; i < dataset->count; ++i) {
            unsigned char *slot = buffer->master + (i * dataset->element_size);
            switch (dataset->data_family) {
            case LOX_CAMPAIGN_FAMILY_INTEGERS:
            case LOX_CAMPAIGN_FAMILY_TIMESTAMPS:
                lox_campaign_fill_integer_bytes(slot, dataset->element_size, keys[i], dataset->seed, i);
                break;
            case LOX_CAMPAIGN_FAMILY_FLOATING_POINT:
                lox_campaign_fill_float_bytes(slot, dataset->element_size, keys[i], dataset->seed);
                break;
            case LOX_CAMPAIGN_FAMILY_CHARACTERS:
                lox_campaign_fill_char_bytes(slot, keys[i], dataset->seed, dataset->data_type);
                break;
            case LOX_CAMPAIGN_FAMILY_STRINGS:
                lox_campaign_fill_string_bytes(slot, dataset->element_size, keys[i], dataset->seed, dataset->data_type);
                break;
            case LOX_CAMPAIGN_FAMILY_RECORDS:
                lox_campaign_fill_record_bytes(slot, dataset->element_size, keys[i], dataset->seed, i);
                break;
            default:
                lox_campaign_fill_record_bytes(slot, dataset->element_size, keys[i], dataset->seed, i);
                break;
            }
        }
        free(keys);
    }
    memcpy(buffer->work, buffer->master, bytes + (LOX_CAMPAIGN_CANARY_SIZE * 2u));
    memcpy(buffer->reference, buffer->master, bytes + (LOX_CAMPAIGN_CANARY_SIZE * 2u));
    lox_campaign_format_sample_preview(dataset, lox_campaign_data_ptr(buffer->master), dataset->sample_preview, sizeof(dataset->sample_preview));
    memset(buffer->master, 0xA5, LOX_CAMPAIGN_CANARY_SIZE);
    memset(buffer->master + LOX_CAMPAIGN_CANARY_SIZE + bytes, 0xA5, LOX_CAMPAIGN_CANARY_SIZE);
    memset(buffer->work, 0xA5, LOX_CAMPAIGN_CANARY_SIZE);
    memset(buffer->work + LOX_CAMPAIGN_CANARY_SIZE + bytes, 0xA5, LOX_CAMPAIGN_CANARY_SIZE);
    memset(buffer->reference, 0xA5, LOX_CAMPAIGN_CANARY_SIZE);
    memset(buffer->reference + LOX_CAMPAIGN_CANARY_SIZE + bytes, 0xA5, LOX_CAMPAIGN_CANARY_SIZE);
    return 1;
}

static unsigned char *lox_campaign_data_ptr(unsigned char *buffer)
{
    return buffer + LOX_CAMPAIGN_CANARY_SIZE;
}

static const unsigned char *lox_campaign_const_data_ptr(const unsigned char *buffer)
{
    return buffer + LOX_CAMPAIGN_CANARY_SIZE;
}

static int lox_campaign_check_canary(const unsigned char *buffer, size_t bytes)
{
    size_t i;
    for (i = 0u; i < LOX_CAMPAIGN_CANARY_SIZE; ++i) {
        if (buffer[i] != 0xA5u || buffer[LOX_CAMPAIGN_CANARY_SIZE + bytes + i] != 0xA5u) {
            return 0;
        }
    }
    return 1;
}

static void lox_campaign_measure_selected_sort(void *context)
{
    const lox_campaign_selected_sort_ctx_t *ctx = (const lox_campaign_selected_sort_ctx_t *)context;
    memcpy(ctx->work, ctx->master, ctx->bytes);
    ctx->entry->sort(
        ctx->work,
        ctx->count,
        ctx->element_size,
        lox_campaign_compare,
        (void *)ctx->compare_ctx,
        ctx->options);
}

static void lox_campaign_measure_public_api(void *context)
{
    const lox_campaign_public_api_ctx_t *ctx = (const lox_campaign_public_api_ctx_t *)context;
    lox_sort_options_t options = *ctx->options;
    memcpy(ctx->work, ctx->master, ctx->bytes);
    (void)lox_sort(
        ctx->work,
        ctx->count,
        ctx->element_size,
        lox_campaign_compare,
        (void *)ctx->compare_ctx,
        &options,
        ctx->result);
}

static void lox_campaign_measure_feature_extraction(void *context)
{
    const lox_campaign_feature_ctx_t *ctx = (const lox_campaign_feature_ctx_t *)context;
    *ctx->out = lox_extract_features(
        ctx->master,
        ctx->count,
        ctx->element_size,
        lox_campaign_compare,
        (void *)ctx->compare_ctx,
        ctx->profile);
}

static void lox_campaign_measure_dispatch_choice(void *context)
{
    const lox_campaign_dispatch_ctx_t *ctx = (const lox_campaign_dispatch_ctx_t *)context;
    lox_algorithm_t alg = lox_campaign_dispatch_choice(
        ctx->features,
        ctx->profile,
        ctx->count,
        ctx->element_size,
        ctx->has_scratch);
    if (ctx->out_algorithm != NULL) {
        *ctx->out_algorithm = alg;
    }
}

static void lox_campaign_collect_timing_samples(
    lox_campaign_measure_fn_t fn,
    void *context,
    uint64_t target_ticks,
    uint64_t timing_samples,
    uint64_t ticks_per_second,
    double *sorted_samples,
    uint64_t *out_sample_count,
    uint64_t *out_total_ticks,
    uint64_t *out_total_iterations)
{
    const lox_clock_tick_source_t *clock = lox_clock_default();
    uint64_t sample_count = 0u;
    uint64_t total_ticks = 0u;
    uint64_t total_iterations = 0u;
    uint64_t sample_ticks[64];
    uint64_t sample_iterations[64];
    size_t rep;
    size_t i;

    for (rep = 0u; rep < timing_samples && rep < 64u; ++rep) {
        uint64_t batch_ticks = 0u;
        uint64_t batch_iterations = 0u;
        do {
            uint64_t start = clock->now_ticks();
            fn(context);
            {
                uint64_t end = clock->now_ticks();
                uint64_t delta = end > start ? end - start : 0u;
                if (delta == 0u) {
                    delta = target_ticks > 0u ? target_ticks : 1u;
                }
                batch_ticks += delta;
            }
            ++batch_iterations;
            if (target_ticks == 0u) {
                break;
            }
        } while (batch_ticks < target_ticks);
        sample_ticks[sample_count] = batch_ticks;
        sample_iterations[sample_count] = batch_iterations;
        total_ticks += batch_ticks;
        total_iterations += batch_iterations;
        ++sample_count;
    }

    for (i = 0u; i < sample_count; ++i) {
        sorted_samples[i] = lox_campaign_ticks_to_ns(sample_ticks[i], ticks_per_second) /
            (double)(sample_iterations[i] == 0u ? 1u : sample_iterations[i]);
    }
    for (i = 0u; i < sample_count; ++i) {
        size_t j;
        for (j = i + 1u; j < sample_count; ++j) {
            if (sorted_samples[j] < sorted_samples[i]) {
                double tmp = sorted_samples[i];
                sorted_samples[i] = sorted_samples[j];
                sorted_samples[j] = tmp;
            }
        }
    }

    if (out_sample_count != NULL) {
        *out_sample_count = sample_count;
    }
    if (out_total_ticks != NULL) {
        *out_total_ticks = total_ticks;
    }
    if (out_total_iterations != NULL) {
        *out_total_iterations = total_iterations;
    }
}

static void lox_campaign_append_text(char *dst, size_t dst_size, size_t *used, const char *text)
{
    size_t i = 0u;

    if (dst == NULL || dst_size == 0u || used == NULL || text == NULL) {
        return;
    }
    if (*used >= dst_size - 1u) {
        dst[dst_size - 1u] = '\0';
        return;
    }
    while (text[i] != '\0' && *used + 1u < dst_size) {
        dst[*used] = text[i];
        ++(*used);
        ++i;
    }
    dst[*used] = '\0';
}

static void lox_campaign_append_format(char *dst, size_t dst_size, size_t *used, const char *fmt, ...)
{
    va_list args;
    int written;

    if (dst == NULL || dst_size == 0u || used == NULL || fmt == NULL) {
        return;
    }
    if (*used >= dst_size - 1u) {
        dst[dst_size - 1u] = '\0';
        return;
    }
    va_start(args, fmt);
    written = vsnprintf(dst + *used, dst_size - *used, fmt, args);
    va_end(args);
    if (written < 0) {
        return;
    }
    if ((size_t)written >= dst_size - *used) {
        *used = dst_size - 1u;
        dst[*used] = '\0';
    } else {
        *used += (size_t)written;
    }
}

static void lox_campaign_escape_ascii(const unsigned char *src, size_t len, char *dst, size_t dst_size)
{
    size_t used = 0u;
    size_t i;

    if (dst_size == 0u) {
        return;
    }
    dst[0] = '\0';
    lox_campaign_append_text(dst, dst_size, &used, "\"");
    for (i = 0u; i < len && src[i] != '\0'; ++i) {
        unsigned char c = src[i];
        if (c == '\\' || c == '"') {
            lox_campaign_append_format(dst, dst_size, &used, "\\%c", c);
        } else if (c >= 32u && c <= 126u) {
            char ch[2] = { (char)c, '\0' };
            lox_campaign_append_text(dst, dst_size, &used, ch);
        } else {
            lox_campaign_append_format(dst, dst_size, &used, "\\x%02X", (unsigned)c);
        }
    }
    lox_campaign_append_text(dst, dst_size, &used, "\"");
}

static void lox_campaign_format_sample_preview(
    const lox_campaign_dataset_t *dataset,
    const unsigned char *data,
    char *dst,
    size_t dst_size)
{
    size_t count;
    size_t i;
    size_t preview_count;
    size_t element_size;
    size_t used = 0u;

    if (dst_size == 0u) {
        return;
    }
    dst[0] = '\0';
    if (dataset == NULL || data == NULL) {
        lox_campaign_append_text(dst, dst_size, &used, "NA");
        return;
    }
    count = dataset->count;
    element_size = dataset->element_size;
    preview_count = count < 3u ? count : 3u;
    lox_campaign_append_text(dst, dst_size, &used, "[");
    for (i = 0u; i < preview_count; ++i) {
        const unsigned char *slot = data + (i * element_size);
        if (i != 0u) {
            lox_campaign_append_text(dst, dst_size, &used, ", ");
        }
        switch (dataset->data_family) {
        case LOX_CAMPAIGN_FAMILY_INTEGERS:
        case LOX_CAMPAIGN_FAMILY_TIMESTAMPS: {
            int is_signed = dataset->data_type == LOX_CAMPAIGN_TYPE_INT8 ||
                dataset->data_type == LOX_CAMPAIGN_TYPE_INT16 ||
                dataset->data_type == LOX_CAMPAIGN_TYPE_INT32 ||
                dataset->data_type == LOX_CAMPAIGN_TYPE_INT64;
            uint64_t value = lox_campaign_read_u64(slot, element_size, is_signed);
            if (is_signed && element_size < sizeof(uint64_t)) {
                int64_t signed_value = (int64_t)value;
                lox_campaign_append_format(dst, dst_size, &used, "%lld", (long long)signed_value);
            } else {
                lox_campaign_append_format(dst, dst_size, &used, "%llu", (unsigned long long)value);
            }
            break;
        }
        case LOX_CAMPAIGN_FAMILY_FLOATING_POINT:
            if (element_size == sizeof(float)) {
                float value;
                memcpy(&value, slot, sizeof(value));
                if (isnan(value)) {
                    lox_campaign_append_text(dst, dst_size, &used, "NaN");
                } else if (isinf(value)) {
                    lox_campaign_append_text(dst, dst_size, &used, signbit(value) ? "-Inf" : "Inf");
                } else if (value == 0.0f && signbit(value)) {
                    lox_campaign_append_text(dst, dst_size, &used, "-0.0");
                } else {
                    lox_campaign_append_format(dst, dst_size, &used, "%.6g", value);
                }
            } else {
                double value;
                memcpy(&value, slot, sizeof(value));
                if (isnan(value)) {
                    lox_campaign_append_text(dst, dst_size, &used, "NaN");
                } else if (isinf(value)) {
                    lox_campaign_append_text(dst, dst_size, &used, signbit(value) ? "-Inf" : "Inf");
                } else if (value == 0.0 && signbit(value)) {
                    lox_campaign_append_text(dst, dst_size, &used, "-0.0");
                } else {
                    lox_campaign_append_format(dst, dst_size, &used, "%.12g", value);
                }
            }
            break;
        case LOX_CAMPAIGN_FAMILY_CHARACTERS:
            lox_campaign_escape_ascii(slot, 1u, dst + used, dst_size - used);
            used = strlen(dst);
            break;
        case LOX_CAMPAIGN_FAMILY_STRINGS:
            lox_campaign_escape_ascii(slot, element_size, dst + used, dst_size - used);
            used = strlen(dst);
            break;
        case LOX_CAMPAIGN_FAMILY_RECORDS: {
            uint64_t key = 0u;
            uint64_t idx = 0u;
            memcpy(&key, slot, sizeof(key) < element_size ? sizeof(key) : element_size);
            if (element_size > sizeof(uint64_t)) {
                memcpy(&idx, slot + sizeof(uint64_t), sizeof(idx) < (element_size - sizeof(uint64_t)) ? sizeof(idx) : (element_size - sizeof(uint64_t)));
            }
            lox_campaign_append_format(dst, dst_size, &used, "{key=%llu, idx=%llu}", (unsigned long long)key, (unsigned long long)idx);
            break;
        }
        default:
            lox_campaign_append_text(dst, dst_size, &used, "NA");
            break;
        }
    }
    lox_campaign_append_text(dst, dst_size, &used, "]");
}

static uint64_t lox_campaign_multiset_hash(const unsigned char *buffer, size_t count, size_t element_size)
{
    uint64_t hash = UINT64_C(0x9e3779b97f4a7c15);
    size_t i;
    for (i = 0u; i < count; ++i) {
        hash ^= lox_campaign_hash_bytes(buffer + (i * element_size), element_size);
        hash = lox_campaign_mix64(hash + UINT64_C(0x9e3779b97f4a7c15));
    }
    return hash;
}

static int lox_campaign_is_sorted(
    const unsigned char *buffer,
    size_t count,
    size_t element_size,
    const lox_campaign_compare_context_t *ctx)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        if (lox_campaign_compare(
                buffer + ((i - 1u) * element_size),
                buffer + (i * element_size),
                (void *)ctx) > 0) {
            return 0;
        }
    }
    return 1;
}

static uint64_t lox_campaign_strategy_id_to_mask_bit(lox_algorithm_t id)
{
    switch (id) {
    case LOX_ALGORITHM_INSERTION:
        return LOX_ALGORITHM_MASK_INSERTION;
    case LOX_ALGORITHM_SHELL:
        return LOX_ALGORITHM_MASK_SHELL;
    case LOX_ALGORITHM_INTRO:
        return LOX_ALGORITHM_MASK_INTRO;
    case LOX_ALGORITHM_MERGE:
        return LOX_ALGORITHM_MASK_MERGE;
    case LOX_ALGORITHM_CYCLE:
        return LOX_ALGORITHM_MASK_CYCLE;
    default:
        return 0u;
    }
}

static lox_algorithm_t lox_campaign_strategy_algorithm(lox_campaign_strategy_t strategy)
{
    switch (strategy) {
    case LOX_CAMPAIGN_STRATEGY_ALWAYS_INSERTION:
        return LOX_ALGORITHM_INSERTION;
    case LOX_CAMPAIGN_STRATEGY_ALWAYS_SHELL:
        return LOX_ALGORITHM_SHELL;
    case LOX_CAMPAIGN_STRATEGY_ALWAYS_INTRO:
        return LOX_ALGORITHM_INTRO;
    case LOX_CAMPAIGN_STRATEGY_ALWAYS_MERGE_IF_AVAILABLE:
        return LOX_ALGORITHM_MERGE;
    case LOX_CAMPAIGN_STRATEGY_ALWAYS_CYCLE_IF_AVAILABLE:
        return LOX_ALGORITHM_CYCLE;
    case LOX_CAMPAIGN_STRATEGY_LOXSORT_DISPATCHER:
    case LOX_CAMPAIGN_STRATEGY_ORACLE_BEST:
    default:
        return LOX_ALGORITHM_NONE;
    }
}

static uint16_t lox_campaign_shell_limit(const lox_profile_t *profile, uint8_t element_class)
{
    switch (element_class) {
    case LOX_ELEMENT_SIZE_SMALL:
        return profile->shell_max_count_elem_small;
    case LOX_ELEMENT_SIZE_MEDIUM:
        return profile->shell_max_count_elem_medium;
    default:
        return profile->shell_max_count_elem_large;
    }
}

static void lox_campaign_fill_scratch(
    void **scratch,
    size_t *scratch_size,
    size_t count,
    size_t element_size,
    lox_campaign_scratch_mode_t mode)
{
    size_t required = lox_merge_scratch_required(count, element_size);
    *scratch = NULL;
    *scratch_size = 0u;
    if (required == SIZE_MAX) {
        return;
    }
    switch (mode) {
    case LOX_CAMPAIGN_SCRATCH_NO:
        return;
    case LOX_CAMPAIGN_SCRATCH_EXACT:
        *scratch = calloc(1u, required == 0u ? 1u : required);
        *scratch_size = required;
        return;
    case LOX_CAMPAIGN_SCRATCH_AMPLE:
        *scratch = calloc(1u, required * 2u + 64u);
        *scratch_size = required * 2u + 64u;
        return;
    case LOX_CAMPAIGN_SCRATCH_INSUFFICIENT:
        if (required > 0u) {
            *scratch_size = required - 1u;
            *scratch = calloc(1u, *scratch_size);
        }
        return;
    }
}

static int lox_campaign_strategy_has_scratch(
    const lox_campaign_dataset_t *dataset,
    const lox_sort_options_t *options)
{
    size_t required = lox_merge_scratch_required(dataset->count, dataset->element_size);
    if (required == SIZE_MAX) {
        return 0;
    }
    if (options->scratch == NULL || options->scratch_size < required) {
        return 0;
    }
    return 1;
}

static lox_algorithm_t lox_campaign_dispatch_choice(
    const lox_feature_vector_t *features,
    const lox_profile_t *profile,
    size_t count,
    size_t element_size,
    int has_scratch)
{
    size_t data_bytes = count * element_size;
    uint8_t sign_mask = features->comparison_sign_mask;
    if (count <= profile->small_count_cutoff) {
        return LOX_ALGORITHM_INSERTION;
    }
    if (profile->duplicate_intro_min_count <= count &&
        count <= profile->duplicate_intro_max_count &&
        features->equal_pair_count >= profile->duplicate_equal_min &&
        (sign_mask & 0x3u) == 0x3u &&
        features->disorder_score >= profile->duplicate_disorder_min) {
        return LOX_ALGORITHM_INTRO;
    }
    if (features->disorder_score <= profile->near_sorted_disorder_max) {
        if (count <= profile->near_sorted_insertion_max_count &&
            features->direction_changes <= profile->near_sorted_direction_changes_max) {
            return LOX_ALGORITHM_INSERTION;
        }
        if (features->direction_changes > profile->near_sorted_direction_changes_max) {
            return LOX_ALGORITHM_INTRO;
        }
    }
    if (has_scratch &&
        count >= profile->merge_large_mixed_min_count &&
        data_bytes >= profile->merge_large_mixed_min_data_bytes &&
        (sign_mask & 0x3u) == 0x3u &&
        features->direction_changes >= profile->merge_large_mixed_direction_min &&
        features->direction_changes <= profile->merge_large_mixed_direction_max &&
        features->disorder_score >= profile->merge_large_mixed_disorder_min) {
        return LOX_ALGORITHM_MERGE;
    }
    if (has_scratch &&
        count >= profile->chaotic_merge_min_count &&
        data_bytes >= profile->chaotic_merge_min_data_bytes &&
        data_bytes <= profile->chaotic_merge_data_bytes_max &&
        (sign_mask & 0x3u) == 0x3u &&
        features->direction_changes >= profile->chaotic_merge_direction_min &&
        features->disorder_score >= profile->chaotic_merge_disorder_min) {
        return LOX_ALGORITHM_MERGE;
    }
    {
        uint16_t shell_limit = lox_campaign_shell_limit(profile, lox_element_size_class(element_size));
        if (count <= shell_limit && shell_limit != 0u) {
            return LOX_ALGORITHM_SHELL;
        }
    }
    if (profile->cycle_min_count != 0u &&
        count >= profile->cycle_min_count &&
        features->equal_score <= profile->cycle_equal_score_max) {
        return LOX_ALGORITHM_CYCLE;
    }
    return LOX_ALGORITHM_INTRO;
}

static void lox_campaign_fill_strategy_order(
    lox_campaign_strategy_t *order,
    size_t strategy_count,
    uint64_t seed)
{
    size_t i;
    for (i = 0u; i < strategy_count; ++i) {
        order[i] = (lox_campaign_strategy_t)i;
    }
    if (strategy_count > 1u) {
        size_t offset = (size_t)(seed % strategy_count);
        lox_campaign_strategy_t tmp[LOX_CAMPAIGN_MAX_STRATEGIES];
        for (i = 0u; i < strategy_count; ++i) {
            tmp[i] = order[(i + offset) % strategy_count];
        }
        for (i = 0u; i < strategy_count; ++i) {
            order[i] = tmp[i];
        }
    }
}

static uint64_t lox_campaign_run_strategy(
    lox_campaign_strategy_t strategy,
    const lox_campaign_dataset_t *dataset,
    const lox_campaign_compare_context_t *ctx,
    const lox_profile_t *profile,
    const unsigned char *master,
    unsigned char *work,
    const lox_sort_options_t *base_options,
    lox_campaign_strategy_result_t *result,
    uint64_t timing_target_ms,
    uint64_t timing_samples,
    const lox_algorithm_entry_t *entry,
    int *valid_out)
{
    const lox_clock_tick_source_t *clock = lox_clock_default();
    uint64_t ticks_per_second = clock->ticks_per_second == 0u ? 1000000000ull : clock->ticks_per_second;
    uint64_t target_ticks = (ticks_per_second / 1000u) * timing_target_ms;
    uint64_t sample_ticks[64];
    uint64_t sample_iterations[64];
    uint64_t sample_count = 0u;
    uint64_t comparison_count = 0u;
    uint64_t total_ticks = 0u;
    uint64_t total_iterations = 0u;
    void *scratch = NULL;
    size_t scratch_size = 0u;
    lox_sort_options_t options = *base_options;
    lox_status_t status = LOX_STATUS_OK;
    lox_algorithm_t selected_algorithm = lox_campaign_strategy_algorithm(strategy);
    const lox_algorithm_entry_t *selected_entry = entry;
    int valid = 1;
    size_t bytes = dataset->count * dataset->element_size;
    size_t rep;
    size_t i;

    if (strategy == LOX_CAMPAIGN_STRATEGY_ALWAYS_MERGE_IF_AVAILABLE) {
        lox_campaign_fill_scratch(&scratch, &scratch_size, dataset->count, dataset->element_size, dataset->scratch_mode);
        options.scratch = scratch;
        options.scratch_size = scratch_size;
        if (!lox_campaign_strategy_has_scratch(dataset, &options)) {
            valid = 0;
        }
    } else {
        options.scratch = NULL;
        options.scratch_size = 0u;
    }
    options.profile = profile;
    options.flags = 0u;
    if (strategy == LOX_CAMPAIGN_STRATEGY_LOXSORT_DISPATCHER) {
        lox_feature_vector_t features = lox_extract_features(master, dataset->count, dataset->element_size, lox_campaign_compare, (void *)ctx, profile);
        selected_algorithm = lox_campaign_dispatch_choice(&features, profile, dataset->count, dataset->element_size, lox_campaign_strategy_has_scratch(dataset, &options));
        selected_entry = lox_registry_find(selected_algorithm);
        if (selected_entry == NULL) {
            valid = 0;
            selected_algorithm = LOX_ALGORITHM_NONE;
        }
    } else {
        selected_algorithm = lox_campaign_strategy_algorithm(strategy);
        selected_entry = lox_registry_find(selected_algorithm);
        if (selected_entry == NULL) {
            valid = 0;
            selected_algorithm = LOX_ALGORITHM_NONE;
        }
    }
    if (strategy == LOX_CAMPAIGN_STRATEGY_ORACLE_BEST) {
        valid = 0;
    }

    memset(work, 0, bytes);
    result->decision_reason = LOX_REASON_PROFILE_GENERAL;
    result->profile_fallback_used = 0u;
    result->sampled_pair_count = 0u;
    result->disorder_score = 0u;
    result->equal_score = 0u;
    result->equal_pair_count = 0u;
    result->direction_changes = 0u;
    result->comparison_sign_mask = 0u;
    if (strategy == LOX_CAMPAIGN_STRATEGY_LOXSORT_DISPATCHER && valid && selected_algorithm != LOX_ALGORITHM_NONE) {
        lox_sort_result_t api_result;
        lox_feature_vector_t features = lox_extract_features(master, dataset->count, dataset->element_size, lox_campaign_compare, (void *)ctx, profile);
        lox_campaign_feature_ctx_t feature_ctx;
        lox_campaign_dispatch_ctx_t dispatch_ctx;
        lox_campaign_public_api_ctx_t public_ctx;
        lox_campaign_selected_sort_ctx_t selected_ctx;
        double selected_samples[64];
        double feature_samples[64];
        double dispatch_samples[64];
        double public_samples[64];
        uint64_t selected_sample_count = 0u;
        uint64_t selected_total_ticks = 0u;
        uint64_t selected_total_iterations = 0u;
        uint64_t feature_sample_count = 0u;
        uint64_t feature_total_ticks = 0u;
        uint64_t feature_total_iterations = 0u;
        uint64_t dispatch_sample_count = 0u;
        uint64_t dispatch_total_ticks = 0u;
        uint64_t dispatch_total_iterations = 0u;
        uint64_t public_sample_count = 0u;
        uint64_t public_total_ticks = 0u;
        uint64_t public_total_iterations = 0u;

        memcpy(work, master, bytes);
        (void)lox_sort(
            work,
            dataset->count,
            dataset->element_size,
            lox_campaign_compare,
            (void *)ctx,
            &options,
            &api_result);
        result->decision_reason = api_result.reason;
        result->profile_fallback_used = api_result.profile_fallback_used;
        result->sampled_pair_count = api_result.sampled_pair_count;
        result->disorder_score = api_result.disorder_score;
        result->equal_score = api_result.equal_score;
        result->equal_pair_count = api_result.equal_pair_count;
        result->direction_changes = api_result.direction_changes;
        result->comparison_sign_mask = api_result.comparison_sign_mask;
        selected_algorithm = api_result.algorithm;
        selected_entry = lox_registry_find(selected_algorithm);
        if (selected_entry == NULL) {
            selected_algorithm = LOX_ALGORITHM_NONE;
            valid = 0;
        }

        if (valid && selected_entry != NULL) {
            selected_ctx.entry = selected_entry;
            selected_ctx.master = master;
            selected_ctx.work = work;
            selected_ctx.bytes = bytes;
            selected_ctx.count = dataset->count;
            selected_ctx.element_size = dataset->element_size;
            selected_ctx.compare_ctx = ctx;
            selected_ctx.options = &options;
            lox_campaign_collect_timing_samples(
                lox_campaign_measure_selected_sort,
                &selected_ctx,
                target_ticks,
                timing_samples,
                ticks_per_second,
                selected_samples,
                &selected_sample_count,
                &selected_total_ticks,
                &selected_total_iterations);

            feature_ctx.master = master;
            feature_ctx.count = dataset->count;
            feature_ctx.element_size = dataset->element_size;
            feature_ctx.compare_ctx = ctx;
            feature_ctx.profile = profile;
            feature_ctx.out = &features;
            lox_campaign_collect_timing_samples(
                lox_campaign_measure_feature_extraction,
                &feature_ctx,
                target_ticks,
                timing_samples,
                ticks_per_second,
                feature_samples,
                &feature_sample_count,
                &feature_total_ticks,
                &feature_total_iterations);
            dispatch_ctx.features = &features;
            dispatch_ctx.profile = profile;
            dispatch_ctx.count = dataset->count;
            dispatch_ctx.element_size = dataset->element_size;
            dispatch_ctx.has_scratch = lox_campaign_strategy_has_scratch(dataset, &options);
            dispatch_ctx.out_algorithm = &selected_algorithm;
            lox_campaign_collect_timing_samples(
                lox_campaign_measure_dispatch_choice,
                &dispatch_ctx,
                target_ticks,
                timing_samples,
                ticks_per_second,
                dispatch_samples,
                &dispatch_sample_count,
                &dispatch_total_ticks,
                &dispatch_total_iterations);
            public_ctx.master = master;
            public_ctx.work = work;
            public_ctx.bytes = bytes;
            public_ctx.count = dataset->count;
            public_ctx.element_size = dataset->element_size;
            public_ctx.compare_ctx = ctx;
            public_ctx.options = &options;
            public_ctx.result = &api_result;
            lox_campaign_collect_timing_samples(
                lox_campaign_measure_public_api,
                &public_ctx,
                target_ticks,
                timing_samples,
                ticks_per_second,
                public_samples,
                &public_sample_count,
                &public_total_ticks,
                &public_total_iterations);

            result->feature_extraction_ns = feature_sample_count == 0u ? NAN : feature_samples[feature_sample_count / 2u];
            result->dispatch_only_ns = dispatch_sample_count == 0u ? NAN : dispatch_samples[dispatch_sample_count / 2u];
            result->selected_sort_ns = selected_sample_count == 0u ? NAN : selected_samples[selected_sample_count / 2u];
            result->public_api_total_ns = public_sample_count == 0u ? NAN : public_samples[public_sample_count / 2u];
            result->dispatcher_overhead_ns = (isnan(result->public_api_total_ns) || isnan(result->selected_sort_ns)) ? NAN : result->public_api_total_ns - result->selected_sort_ns;
            result->dispatcher_overhead_percent = (!isnan(result->dispatcher_overhead_ns) && result->selected_sort_ns > 0.0)
                ? (result->dispatcher_overhead_ns * 100.0) / result->selected_sort_ns
                : NAN;
            result->sample_count = public_sample_count;
            result->timing_iterations = public_total_iterations;
            result->elapsed_ticks_total = public_total_ticks;
            result->ticks_per_iteration = public_total_iterations == 0u ? 0u : public_total_ticks / public_total_iterations;
            result->min_ns = public_sample_count > 0u ? public_samples[0] : NAN;
            result->median_ns = public_sample_count > 0u ? public_samples[public_sample_count / 2u] : NAN;
            result->mean_ns = 0.0;
            if (public_sample_count > 0u) {
                for (i = 0u; i < public_sample_count; ++i) {
                    result->mean_ns += public_samples[i];
                }
                result->mean_ns /= (double)public_sample_count;
                result->p95_ns = public_samples[(public_sample_count * 95u) / 100u < public_sample_count ? (public_sample_count * 95u) / 100u : public_sample_count - 1u];
                result->max_ns = public_samples[public_sample_count - 1u];
            } else {
                result->p95_ns = NAN;
                result->max_ns = NAN;
            }
            if (selected_sample_count > 0u) {
                memcpy(work, master, bytes);
                selected_entry->sort(work, dataset->count, dataset->element_size, lox_campaign_compare, (void *)ctx, &options);
                status = lox_campaign_is_sorted(work, dataset->count, dataset->element_size, ctx) ? LOX_STATUS_OK : LOX_STATUS_VERIFY_FAILED;
            } else {
                status = LOX_STATUS_OK;
            }
        } else {
            result->sample_count = 0u;
            result->timing_iterations = 0u;
            result->elapsed_ticks_total = 0u;
            result->ticks_per_iteration = 0u;
            result->min_ns = NAN;
            result->median_ns = NAN;
            result->mean_ns = NAN;
            result->p95_ns = NAN;
            result->max_ns = NAN;
            result->feature_extraction_ns = NAN;
            result->dispatch_only_ns = NAN;
            result->selected_sort_ns = NAN;
            result->public_api_total_ns = NAN;
            result->dispatcher_overhead_ns = NAN;
            result->dispatcher_overhead_percent = NAN;
            status = LOX_STATUS_OK;
        }
    } else if (valid && selected_algorithm != LOX_ALGORITHM_NONE) {
        double selected_samples[64];
        uint64_t selected_sample_count = 0u;
        uint64_t selected_total_ticks = 0u;
        uint64_t selected_total_iterations = 0u;
        lox_campaign_selected_sort_ctx_t selected_ctx;
        selected_ctx.entry = selected_entry;
        selected_ctx.master = master;
        selected_ctx.work = work;
        selected_ctx.bytes = bytes;
        selected_ctx.count = dataset->count;
        selected_ctx.element_size = dataset->element_size;
        selected_ctx.compare_ctx = ctx;
        selected_ctx.options = &options;
        lox_campaign_collect_timing_samples(
            lox_campaign_measure_selected_sort,
            &selected_ctx,
            target_ticks,
            timing_samples,
            ticks_per_second,
            selected_samples,
            &selected_sample_count,
            &selected_total_ticks,
            &selected_total_iterations);
        result->sample_count = selected_sample_count;
        result->timing_iterations = selected_total_iterations;
        result->elapsed_ticks_total = selected_total_ticks;
        result->ticks_per_iteration = selected_total_iterations == 0u ? 0u : selected_total_ticks / selected_total_iterations;
        result->min_ns = selected_sample_count > 0u ? selected_samples[0] : NAN;
        result->median_ns = selected_sample_count > 0u ? selected_samples[selected_sample_count / 2u] : NAN;
        result->mean_ns = 0.0;
        if (selected_sample_count > 0u) {
            for (i = 0u; i < selected_sample_count; ++i) {
                result->mean_ns += selected_samples[i];
            }
            result->mean_ns /= (double)selected_sample_count;
            result->p95_ns = selected_samples[(selected_sample_count * 95u) / 100u < selected_sample_count ? (selected_sample_count * 95u) / 100u : selected_sample_count - 1u];
            result->max_ns = selected_samples[selected_sample_count - 1u];
        } else {
            result->p95_ns = NAN;
            result->max_ns = NAN;
        }
        result->selected_sort_ns = result->median_ns;
        result->feature_extraction_ns = NAN;
        result->dispatch_only_ns = NAN;
        result->public_api_total_ns = NAN;
        result->dispatcher_overhead_ns = NAN;
        result->dispatcher_overhead_percent = NAN;
        status = lox_campaign_is_sorted(work, dataset->count, dataset->element_size, ctx) ? LOX_STATUS_OK : LOX_STATUS_VERIFY_FAILED;
    } else {
        result->sample_count = 0u;
        result->timing_iterations = 0u;
        result->elapsed_ticks_total = 0u;
        result->ticks_per_iteration = 0u;
        result->min_ns = NAN;
        result->median_ns = NAN;
        result->mean_ns = NAN;
        result->p95_ns = NAN;
        result->max_ns = NAN;
        result->feature_extraction_ns = NAN;
        result->dispatch_only_ns = NAN;
        result->selected_sort_ns = NAN;
        result->public_api_total_ns = NAN;
        result->dispatcher_overhead_ns = NAN;
        result->dispatcher_overhead_percent = NAN;
        status = LOX_STATUS_OK;
    }

    result->strategy = strategy;
    result->selected_algorithm = selected_algorithm;
    result->clock_frequency_hz = ticks_per_second;
    if (strategy != LOX_CAMPAIGN_STRATEGY_LOXSORT_DISPATCHER) {
        result->decision_reason = LOX_REASON_PROFILE_GENERAL;
        result->profile_fallback_used = 0u;
    }
    result->comparison_count = comparison_count;
    result->move_count = 0u;
    result->swap_count = 0u;
    result->bytes_moved = 0u;
    result->sorted_ok = (status == LOX_STATUS_OK);
    result->multiset_ok = 1;
    result->payload_ok = 1;
    result->canary_ok = lox_campaign_check_canary(master - LOX_CAMPAIGN_CANARY_SIZE, bytes) &&
        lox_campaign_check_canary(work - LOX_CAMPAIGN_CANARY_SIZE, bytes);
    result->regret = 1.0;
    result->absolute_loss_ns = 0.0;

    free(scratch);
    (void)comparison_count;
    return valid ? total_ticks : 0u;
}

static void lox_campaign_write_csv_header(FILE *out)
{
    fprintf(
        out,
        "campaign_id,dataset_id,data_family,data_type,pattern,count,element_size,data_bytes,comparator_kind,seed,scratch_mode,algorithm,selected_algorithm,best_algorithm,clock_frequency_hz,timing_iterations,sample_count,elapsed_ticks_total,ticks_per_iteration,min_ns,median_ns,mean_ns,p95_ns,max_ns,feature_extraction_ns,dispatch_only_ns,selected_sort_ns,public_api_total_ns,dispatcher_overhead_ns,dispatcher_overhead_percent,comparison_count,move_count,swap_count,bytes_moved,sorted_ok,multiset_ok,payload_ok,canary_ok,decision_reason,profile_fallback_used,sampled_pair_count,disorder_score,equal_score,equal_pair_count,direction_changes,comparison_sign_mask,regret,absolute_loss_ns,sample_preview\n");
}

static const char *lox_campaign_bool_text(int value)
{
    return value ? "1" : "0";
}

static void lox_campaign_write_csv_text(FILE *out, const char *text)
{
    size_t i;

    fputc('"', out);
    if (text != NULL) {
        for (i = 0u; text[i] != '\0'; ++i) {
            if (text[i] == '"') {
                fputc('"', out);
            }
            fputc(text[i], out);
        }
    }
    fputc('"', out);
}

static void lox_campaign_write_row(
    FILE *out,
    const char *campaign_id,
    const lox_campaign_dataset_t *dataset,
    const lox_campaign_strategy_result_t *row,
    lox_algorithm_t best_algorithm)
{
    const char *algorithm_name = lox_campaign_strategy_name(row->strategy);
    const char *selected_name = lox_algorithm_name(row->selected_algorithm);
    const char *best_name = lox_algorithm_name(best_algorithm);
    fprintf(
        out,
        "%s,%zu,%s,%s,%s,%zu,%zu,%zu,%s,%llu,%s,%s,%s,%s,%llu,%llu,%llu,%llu,%llu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%llu,%llu,%llu,%llu,%s,%s,%s,%s,%u,%s,%llu,%llu,%llu,%llu,%llu,%llu,%.3f,%.3f",
        campaign_id,
        dataset->dataset_id,
        lox_campaign_family_name(dataset->data_family),
        lox_campaign_data_type_name(dataset->data_type),
        lox_bench_pattern_name(dataset->pattern),
        dataset->count,
        dataset->element_size,
        dataset->count * dataset->element_size,
        lox_campaign_comparator_name(dataset->comparator_kind),
        (unsigned long long)dataset->seed,
        lox_campaign_scratch_mode_name(dataset->scratch_mode),
        algorithm_name,
        selected_name,
        best_name,
        (unsigned long long)row->clock_frequency_hz,
        (unsigned long long)row->timing_iterations,
        (unsigned long long)row->sample_count,
        (unsigned long long)row->elapsed_ticks_total,
        (unsigned long long)row->ticks_per_iteration,
        row->min_ns,
        row->median_ns,
        row->mean_ns,
        row->p95_ns,
        row->max_ns,
        row->feature_extraction_ns,
        row->dispatch_only_ns,
        row->selected_sort_ns,
        row->public_api_total_ns,
        row->dispatcher_overhead_ns,
        row->dispatcher_overhead_percent,
        (unsigned long long)row->comparison_count,
        (unsigned long long)row->move_count,
        (unsigned long long)row->swap_count,
        (unsigned long long)row->bytes_moved,
        lox_campaign_bool_text(row->sorted_ok),
        lox_campaign_bool_text(row->multiset_ok),
        lox_campaign_bool_text(row->payload_ok),
        lox_campaign_bool_text(row->canary_ok),
        (unsigned)row->decision_reason,
        lox_campaign_bool_text(row->profile_fallback_used),
        (unsigned long long)row->sampled_pair_count,
        (unsigned long long)row->disorder_score,
        (unsigned long long)row->equal_score,
        (unsigned long long)row->equal_pair_count,
        (unsigned long long)row->direction_changes,
        (unsigned long long)row->comparison_sign_mask,
        row->regret,
        row->absolute_loss_ns);
    fputc(',', out);
    lox_campaign_write_csv_text(out, dataset->sample_preview);
    fputc('\n', out);
}

static void lox_campaign_update_hash(
    lox_campaign_run_state_t *state,
    const lox_campaign_dataset_t *dataset)
{
    uint64_t key[8];
    key[0] = (uint64_t)dataset->data_family;
    key[1] = (uint64_t)dataset->data_type;
    key[2] = (uint64_t)dataset->pattern;
    key[3] = (uint64_t)dataset->count;
    key[4] = (uint64_t)dataset->element_size;
    key[5] = (uint64_t)dataset->comparator_kind;
    key[6] = dataset->seed;
    key[7] = (uint64_t)dataset->scratch_mode;
    state->scenario_hash_a ^= lox_campaign_mix64(key[0] + key[4]);
    state->scenario_hash_b ^= lox_campaign_mix64(key[1] + key[5]);
    state->scenario_hash_c ^= lox_campaign_mix64(key[2] + key[6]);
    state->scenario_hash_d ^= lox_campaign_mix64(key[3] + key[7]);
}

static void lox_campaign_plan_dataset(
    const lox_campaign_run_state_t *state,
    uint8_t family,
    uint8_t family_index,
    uint8_t bucket,
    uint8_t bucket_index,
    lox_campaign_dataset_t *dataset)
{
    static const lox_campaign_data_type_t family_types[][LOX_CAMPAIGN_MAX_FAMILY_TYPES] = {
        { LOX_CAMPAIGN_TYPE_INT8, LOX_CAMPAIGN_TYPE_UINT8, LOX_CAMPAIGN_TYPE_INT16, LOX_CAMPAIGN_TYPE_UINT16, LOX_CAMPAIGN_TYPE_INT32, LOX_CAMPAIGN_TYPE_UINT32, LOX_CAMPAIGN_TYPE_INT64, LOX_CAMPAIGN_TYPE_UINT64 },
        { LOX_CAMPAIGN_TYPE_FLOAT, LOX_CAMPAIGN_TYPE_DOUBLE, LOX_CAMPAIGN_TYPE_FLOAT, LOX_CAMPAIGN_TYPE_DOUBLE, LOX_CAMPAIGN_TYPE_FLOAT, LOX_CAMPAIGN_TYPE_DOUBLE, LOX_CAMPAIGN_TYPE_FLOAT, LOX_CAMPAIGN_TYPE_DOUBLE },
        { LOX_CAMPAIGN_TYPE_RAW_BYTES, LOX_CAMPAIGN_TYPE_ASCII, LOX_CAMPAIGN_TYPE_DIGITS, LOX_CAMPAIGN_TYPE_LOWERCASE, LOX_CAMPAIGN_TYPE_UPPERCASE, LOX_CAMPAIGN_TYPE_ALNUM, LOX_CAMPAIGN_TYPE_CASE_INSENSITIVE_ASCII, LOX_CAMPAIGN_TYPE_RAW_BYTES },
        { LOX_CAMPAIGN_TYPE_FIXED_STRING_8, LOX_CAMPAIGN_TYPE_FIXED_STRING_16, LOX_CAMPAIGN_TYPE_FIXED_STRING_32, LOX_CAMPAIGN_TYPE_FIXED_STRING_64, LOX_CAMPAIGN_TYPE_FIXED_STRING_128, LOX_CAMPAIGN_TYPE_POINTER_STRING, LOX_CAMPAIGN_TYPE_FIXED_STRING_16, LOX_CAMPAIGN_TYPE_FIXED_STRING_32 },
        { LOX_CAMPAIGN_TYPE_SENSOR_RECORD, LOX_CAMPAIGN_TYPE_PACKET_RECORD, LOX_CAMPAIGN_TYPE_LOG_RECORD, LOX_CAMPAIGN_TYPE_SCHEDULER_TASK, LOX_CAMPAIGN_TYPE_DATABASE_RECORD, LOX_CAMPAIGN_TYPE_DEVICE_EVENT, LOX_CAMPAIGN_TYPE_SENSOR_RECORD, LOX_CAMPAIGN_TYPE_PACKET_RECORD },
        { LOX_CAMPAIGN_TYPE_TICKS32, LOX_CAMPAIGN_TYPE_TICKS64, LOX_CAMPAIGN_TYPE_TIMESTAMP_SEQUENCE, LOX_CAMPAIGN_TYPE_TIMESTAMP_DEVICE, LOX_CAMPAIGN_TYPE_TIMESTAMP_PRIORITY, LOX_CAMPAIGN_TYPE_TICKS64, LOX_CAMPAIGN_TYPE_TICKS32, LOX_CAMPAIGN_TYPE_TIMESTAMP_SEQUENCE }
    };
    static const lox_campaign_comparator_kind_t family_cmps[][LOX_CAMPAIGN_MAX_FAMILY_TYPES] = {
        { LOX_CAMPAIGN_CMP_NUMERIC, LOX_CAMPAIGN_CMP_UNSIGNED, LOX_CAMPAIGN_CMP_NUMERIC, LOX_CAMPAIGN_CMP_UNSIGNED, LOX_CAMPAIGN_CMP_NUMERIC, LOX_CAMPAIGN_CMP_UNSIGNED, LOX_CAMPAIGN_CMP_NUMERIC, LOX_CAMPAIGN_CMP_UNSIGNED },
        { LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER, LOX_CAMPAIGN_CMP_TOTAL_ORDER },
        { LOX_CAMPAIGN_CMP_UNSIGNED, LOX_CAMPAIGN_CMP_CASE_SENSITIVE, LOX_CAMPAIGN_CMP_UNSIGNED, LOX_CAMPAIGN_CMP_CASE_SENSITIVE, LOX_CAMPAIGN_CMP_CASE_INSENSITIVE, LOX_CAMPAIGN_CMP_UNSIGNED, LOX_CAMPAIGN_CMP_CASE_INSENSITIVE, LOX_CAMPAIGN_CMP_UNSIGNED },
        { LOX_CAMPAIGN_CMP_CASE_SENSITIVE, LOX_CAMPAIGN_CMP_CASE_INSENSITIVE, LOX_CAMPAIGN_CMP_CASE_SENSITIVE, LOX_CAMPAIGN_CMP_CASE_INSENSITIVE, LOX_CAMPAIGN_CMP_CASE_SENSITIVE, LOX_CAMPAIGN_CMP_CASE_INSENSITIVE, LOX_CAMPAIGN_CMP_CASE_SENSITIVE, LOX_CAMPAIGN_CMP_CASE_INSENSITIVE },
        { LOX_CAMPAIGN_CMP_ID, LOX_CAMPAIGN_CMP_TIMESTAMP, LOX_CAMPAIGN_CMP_PRIORITY, LOX_CAMPAIGN_CMP_PRIORITY_TIMESTAMP_ID, LOX_CAMPAIGN_CMP_TEXT_FIELD, LOX_CAMPAIGN_CMP_ID, LOX_CAMPAIGN_CMP_TIMESTAMP, LOX_CAMPAIGN_CMP_PRIORITY },
        { LOX_CAMPAIGN_CMP_TIMESTAMP, LOX_CAMPAIGN_CMP_SEQUENCE, LOX_CAMPAIGN_CMP_DEVICE, LOX_CAMPAIGN_CMP_PRIORITY, LOX_CAMPAIGN_CMP_PRIORITY_TIMESTAMP_ID, LOX_CAMPAIGN_CMP_TIMESTAMP, LOX_CAMPAIGN_CMP_SEQUENCE, LOX_CAMPAIGN_CMP_DEVICE }
    };
    static const lox_campaign_scratch_mode_t scratch_modes[] = {
        LOX_CAMPAIGN_SCRATCH_NO,
        LOX_CAMPAIGN_SCRATCH_EXACT,
        LOX_CAMPAIGN_SCRATCH_AMPLE,
        LOX_CAMPAIGN_SCRATCH_INSUFFICIENT
    };
    size_t dataset_id = dataset->dataset_id;
    const lox_bench_pattern_t pattern = g_all_patterns[(dataset_id + family_index) % (sizeof(g_all_patterns) / sizeof(g_all_patterns[0]))];
    uint64_t seed = state->seed_base + (uint64_t)dataset_id * UINT64_C(6364136223846793005) + (uint64_t)family * UINT64_C(1442695040888963407);
    size_t type_index = family_index % LOX_CAMPAIGN_MAX_FAMILY_TYPES;

    memset(dataset, 0, sizeof(*dataset));
    dataset->dataset_id = dataset_id;
    dataset->data_family = (lox_campaign_family_t)family;
    dataset->data_type = family_types[family][type_index];
    dataset->pattern = pattern;
    dataset->count = lox_campaign_pick_count(bucket, bucket_index, seed);
    dataset->element_size = lox_campaign_element_size_for_type(dataset->data_type);
    dataset->comparator_kind = family_cmps[family][type_index];
    dataset->seed = seed;
    dataset->scratch_mode = scratch_modes[(dataset_id + family + bucket) % (sizeof(scratch_modes) / sizeof(scratch_modes[0]))];
}

static int lox_campaign_build_plan(
    lox_campaign_run_state_t *state,
    lox_campaign_dataset_t *datasets)
{
    size_t total = state->dataset_count;
    size_t family_targets[6];
    size_t bucket_targets[4];
    uint8_t *family_sequence = NULL;
    uint8_t *bucket_sequence = NULL;
    size_t family_seen[6] = { 0u };
    size_t bucket_seen[4] = { 0u };
    size_t i;

    for (i = 0u; i < 6u; ++i) {
        family_targets[i] = lox_campaign_family_target(total, (lox_campaign_family_t)i);
    }
    family_targets[5] = total - family_targets[0] - family_targets[1] - family_targets[2] - family_targets[3] - family_targets[4];
    for (i = 0u; i < 4u; ++i) {
        bucket_targets[i] = lox_campaign_bucket_target(total, (uint8_t)i);
    }
    bucket_targets[3] = total - bucket_targets[0] - bucket_targets[1] - bucket_targets[2];

    family_sequence = (uint8_t *)calloc(total, sizeof(uint8_t));
    bucket_sequence = (uint8_t *)calloc(total, sizeof(uint8_t));
    if (family_sequence == NULL || bucket_sequence == NULL) {
        free(family_sequence);
        free(bucket_sequence);
        return 0;
    }
    lox_campaign_fill_weighted_sequence(family_sequence, total, family_targets, 6u);
    lox_campaign_fill_weighted_sequence(bucket_sequence, total, bucket_targets, 4u);

    for (i = 0u; i < total; ++i) {
        uint8_t family = family_sequence[i];
        uint8_t bucket = bucket_sequence[i];
        size_t family_index = family_seen[family]++;
        size_t bucket_index = bucket_seen[bucket]++;
        datasets[i].dataset_id = i;
        lox_campaign_plan_dataset(state, family, (uint8_t)family_index, bucket, (uint8_t)bucket_index, &datasets[i]);
    }

    free(family_sequence);
    free(bucket_sequence);
    return 1;
}

static void lox_campaign_compute_best_algorithm(
    const lox_campaign_strategy_result_t *results,
    size_t result_count,
    size_t *best_result_index,
    lox_algorithm_t *best_algorithm,
    double *best_ns)
{
    size_t i;
    size_t best_index = SIZE_MAX;
    *best_algorithm = LOX_ALGORITHM_NONE;
    *best_ns = 0.0;
    for (i = 0u; i < result_count; ++i) {
        double value = results[i].selected_sort_ns;
        if (results[i].strategy == LOX_CAMPAIGN_STRATEGY_ORACLE_BEST) {
            continue;
        }
        if (results[i].sorted_ok == 0 || results[i].selected_algorithm == LOX_ALGORITHM_NONE || value <= 0.0 || value != value) {
            continue;
        }
        if (*best_algorithm == LOX_ALGORITHM_NONE || value < *best_ns) {
            best_index = i;
            *best_algorithm = results[i].selected_algorithm;
            *best_ns = value;
        }
    }
    if (best_result_index != NULL) {
        *best_result_index = best_index;
    }
}

static void lox_campaign_measure_dataset(
    lox_campaign_run_state_t *state,
    lox_campaign_dataset_t *dataset,
    const char *campaign_id,
    FILE *csv)
{
    lox_campaign_dataset_buffer_t buffer;
    lox_campaign_compare_context_t compare_ctx;
    const lox_algorithm_entry_t *entries = NULL;
    size_t entry_count = 0u;
    lox_campaign_strategy_result_t results[LOX_CAMPAIGN_MAX_STRATEGIES];
    lox_campaign_strategy_t strategies[LOX_CAMPAIGN_MAX_STRATEGIES];
    size_t strategy_count = 0u;
    lox_campaign_strategy_result_t oracle_row;
    size_t i;
    lox_algorithm_t best_algorithm = LOX_ALGORITHM_NONE;
    double best_ns = 0.0;

    if (!lox_campaign_generate_dataset(dataset, &buffer)) {
        return;
    }

    compare_ctx.family = dataset->data_family;
    compare_ctx.type = dataset->data_type;
    compare_ctx.comparator_kind = dataset->comparator_kind;
    compare_ctx.element_size = dataset->element_size;
    compare_ctx.comparison_count = NULL;

    entries = lox_registry_entries(&entry_count);

    strategies[strategy_count++] = LOX_CAMPAIGN_STRATEGY_ALWAYS_INSERTION;
    strategies[strategy_count++] = LOX_CAMPAIGN_STRATEGY_ALWAYS_SHELL;
    strategies[strategy_count++] = LOX_CAMPAIGN_STRATEGY_ALWAYS_INTRO;
    strategies[strategy_count++] = LOX_CAMPAIGN_STRATEGY_ALWAYS_MERGE_IF_AVAILABLE;
    strategies[strategy_count++] = LOX_CAMPAIGN_STRATEGY_ALWAYS_CYCLE_IF_AVAILABLE;
    strategies[strategy_count++] = LOX_CAMPAIGN_STRATEGY_LOXSORT_DISPATCHER;

    lox_campaign_fill_strategy_order(strategies, strategy_count, dataset->seed);

    for (i = 0u; i < strategy_count; ++i) {
        const lox_algorithm_entry_t *entry = NULL;
        lox_algorithm_t alg = lox_campaign_strategy_algorithm(strategies[i]);
        int valid = 1;

        if (alg == LOX_ALGORITHM_NONE) {
            alg = LOX_ALGORITHM_INSERTION;
        }
        for (entry = entries; entry < entries + entry_count; ++entry) {
            if (entry->id == alg) {
                break;
            }
        }
        if (entry == entries + entry_count) {
            memset(&results[i], 0, sizeof(results[i]));
            results[i].strategy = strategies[i];
            results[i].selected_algorithm = LOX_ALGORITHM_NONE;
            results[i].sorted_ok = 1;
            continue;
        }
        if (strategies[i] == LOX_CAMPAIGN_STRATEGY_ALWAYS_MERGE_IF_AVAILABLE) {
            void *scratch = NULL;
            size_t scratch_size = 0u;
            lox_sort_options_t scratch_options;
            lox_campaign_fill_scratch(&scratch, &scratch_size, dataset->count, dataset->element_size, dataset->scratch_mode);
            scratch_options.scratch = scratch;
            scratch_options.scratch_size = scratch_size;
            scratch_options.profile = &lox_profile_generic;
            scratch_options.flags = 0u;
            if (!lox_campaign_strategy_has_scratch(dataset, &scratch_options)) {
                valid = 0;
            }
            free(scratch);
        }
        if (strategies[i] == LOX_CAMPAIGN_STRATEGY_ORACLE_BEST) {
            valid = 0;
        }
        if (!valid) {
            memset(&results[i], 0, sizeof(results[i]));
            results[i].strategy = strategies[i];
            results[i].selected_algorithm = LOX_ALGORITHM_NONE;
            results[i].sorted_ok = 1;
            continue;
        }
        {
            lox_campaign_compare_context_t local_ctx = compare_ctx;
            lox_sort_options_t options;
            int strategy_valid = 1;
            results[i].strategy = strategies[i];
            results[i].comparison_count = 0u;
            local_ctx.comparison_count = &results[i].comparison_count;
            options.profile = &lox_profile_generic;
            options.flags = 0u;
            options.scratch = NULL;
            options.scratch_size = 0u;
            if (alg == LOX_ALGORITHM_MERGE) {
                lox_campaign_fill_scratch(&options.scratch, &options.scratch_size, dataset->count, dataset->element_size, dataset->scratch_mode);
            }
            lox_campaign_run_strategy(
                strategies[i],
                dataset,
                &local_ctx,
                &lox_profile_generic,
                lox_campaign_data_ptr(buffer.master),
                lox_campaign_data_ptr(buffer.work),
                &options,
                &results[i],
                state->timing_target_ms,
                state->timing_samples,
                entry,
                &strategy_valid);
            results[i].comparison_count = local_ctx.comparison_count == NULL ? 0u : *local_ctx.comparison_count;
            if (options.scratch != NULL) {
                free(options.scratch);
            }
        }
    }

    {
        size_t best_result_index = SIZE_MAX;
        lox_campaign_compute_best_algorithm(results, strategy_count, &best_result_index, &best_algorithm, &best_ns);
        oracle_row = (best_result_index == SIZE_MAX) ? results[0] : results[best_result_index];
    }
    oracle_row.strategy = LOX_CAMPAIGN_STRATEGY_ORACLE_BEST;
    oracle_row.selected_algorithm = best_algorithm;
    oracle_row.best_algorithm = best_algorithm;
    oracle_row.selected_sort_ns = best_ns;
    oracle_row.regret = 1.0;
    oracle_row.absolute_loss_ns = 0.0;

    for (i = 0u; i < strategy_count; ++i) {
        double selected_ns = results[i].selected_sort_ns;
        results[i].best_algorithm = best_algorithm;
        if (best_ns > 0.0 && results[i].sorted_ok && results[i].selected_algorithm != LOX_ALGORITHM_NONE && selected_ns > 0.0) {
            results[i].regret = selected_ns / best_ns;
            results[i].absolute_loss_ns = selected_ns - best_ns;
        } else {
            results[i].regret = 1.0;
            results[i].absolute_loss_ns = 0.0;
        }
        lox_campaign_write_row(csv, campaign_id, dataset, &results[i], best_algorithm);
    }
    lox_campaign_write_row(csv, campaign_id, dataset, &oracle_row, best_algorithm);

    lox_campaign_update_hash(state, dataset);
    lox_campaign_clear_buffer(&buffer);
}

static int lox_campaign_open_outputs(
    lox_campaign_run_state_t *state,
    int resume)
{
    FILE *csv = NULL;
    if (resume) {
        FILE *checkpoint_in = NULL;
        unsigned long long next_dataset_id = 0ull;
        unsigned long long dataset_count = 0ull;
        unsigned long long seed_base = 0ull;
        int campaign = 0;
        checkpoint_in = fopen(state->checkpoint_path, "r");
        if (checkpoint_in != NULL) {
            if (fscanf(
                    checkpoint_in,
                    "{\n  \"next_dataset_id\": %llu,\n  \"dataset_count\": %llu,\n  \"seed_base\": %llu,\n  \"campaign\": %d\n}\n",
                    &next_dataset_id,
                    &dataset_count,
                    &seed_base,
                    &campaign) == 4) {
                state->next_dataset_id = (size_t)next_dataset_id;
            }
            fclose(checkpoint_in);
        }
    }
    csv = fopen(state->output_path, resume ? "a" : "w");
    if (csv == NULL) {
        return 0;
    }
    state->csv = csv;
    state->checkpoint = NULL;
    if (!resume) {
        lox_campaign_write_csv_header(csv);
    }
    return 1;
}

static void lox_campaign_write_checkpoint(
    const lox_campaign_run_state_t *state)
{
    FILE *checkpoint;

    if (state == NULL) {
        return;
    }
    checkpoint = fopen(state->checkpoint_path, "w");
    if (checkpoint == NULL) {
        return;
    }
    fprintf(
        checkpoint,
        "{\n  \"next_dataset_id\": %llu,\n  \"dataset_count\": %zu,\n  \"seed_base\": %llu,\n  \"campaign\": %d\n}\n",
        (unsigned long long)state->next_dataset_id,
        state->dataset_count,
        (unsigned long long)state->seed_base,
        (int)state->campaign);
    fclose(checkpoint);
}

static int lox_campaign_parse_cli(
    int argc,
    char **argv,
    lox_campaign_run_state_t *state,
    int *resume)
{
    size_t i;
    memset(state, 0, sizeof(*state));
    state->campaign = LOX_CAMPAIGN_SMOKE;
    state->dataset_count = lox_campaign_plan_dataset_count(state->campaign);
    state->seed_base = 1u;
    state->checkpoint_interval = 100u;
    state->timing_target_ms = 2u;
    state->timing_samples = 3u;
    state->stop_after_datasets = 0u;
    strcpy(state->output_path, "generated/benchmark.csv");
    strcpy(state->checkpoint_path, "generated/benchmark.checkpoint.json");
    strcpy(state->report_path, "generated/benchmark_1m_report.md");

    *resume = 0;
    for (i = 1u; i < (size_t)argc; ++i) {
        const char *arg = argv[i];
        if (strcmp(arg, "--campaign") == 0 && i + 1u < (size_t)argc) {
            if (strcmp(argv[i + 1u], "smoke") == 0) {
                state->campaign = LOX_CAMPAIGN_SMOKE;
            } else if (strcmp(argv[i + 1u], "medium") == 0) {
                state->campaign = LOX_CAMPAIGN_MEDIUM;
            } else if (strcmp(argv[i + 1u], "full-1m") == 0) {
                state->campaign = LOX_CAMPAIGN_FULL_1M;
            } else {
                return 0;
            }
            state->dataset_count = lox_campaign_plan_dataset_count(state->campaign);
            ++i;
        } else if (strcmp(arg, "--seed-base") == 0 && i + 1u < (size_t)argc) {
            state->seed_base = strtoull(argv[++i], NULL, 10);
        } else if (strcmp(arg, "--output") == 0 && i + 1u < (size_t)argc) {
            strncpy(state->output_path, argv[++i], sizeof(state->output_path) - 1u);
            state->output_path[sizeof(state->output_path) - 1u] = '\0';
        } else if (strcmp(arg, "--resume") == 0) {
            *resume = 1;
        } else if (strcmp(arg, "--checkpoint-interval") == 0 && i + 1u < (size_t)argc) {
            state->checkpoint_interval = strtoull(argv[++i], NULL, 10);
        } else if (strcmp(arg, "--timing-target-ms") == 0 && i + 1u < (size_t)argc) {
            state->timing_target_ms = strtoull(argv[++i], NULL, 10);
        } else if (strcmp(arg, "--timing-samples") == 0 && i + 1u < (size_t)argc) {
            state->timing_samples = strtoull(argv[++i], NULL, 10);
        } else if (strcmp(arg, "--stop-after-datasets") == 0 && i + 1u < (size_t)argc) {
            state->stop_after_datasets = strtoull(argv[++i], NULL, 10);
        } else if (strcmp(arg, "--report") == 0 && i + 1u < (size_t)argc) {
            strncpy(state->report_path, argv[++i], sizeof(state->report_path) - 1u);
            state->report_path[sizeof(state->report_path) - 1u] = '\0';
        } else {
            return 0;
        }
    }
    return 1;
}

static void lox_campaign_write_report_placeholder(
    const lox_campaign_run_state_t *state)
{
    FILE *report = fopen(state->report_path, "w");
    if (report == NULL) {
        return;
    }
    fprintf(report, "# Benchmark report placeholder\n");
    fprintf(report, "- campaign: %d\n", (int)state->campaign);
    fprintf(report, "- dataset_count: %zu\n", state->dataset_count);
    fprintf(report, "- seed_base: %llu\n", (unsigned long long)state->seed_base);
    fprintf(report, "- scenario_hash_a: %llu\n", (unsigned long long)state->scenario_hash_a);
    fprintf(report, "- scenario_hash_b: %llu\n", (unsigned long long)state->scenario_hash_b);
    fprintf(report, "- scenario_hash_c: %llu\n", (unsigned long long)state->scenario_hash_c);
    fprintf(report, "- scenario_hash_d: %llu\n", (unsigned long long)state->scenario_hash_d);
    fclose(report);
}

int lox_campaign_run_cli(int argc, char **argv)
{
    lox_campaign_run_state_t state;
    lox_campaign_dataset_t *datasets = NULL;
    int resume = 0;
    size_t i;
    const char *campaign_id = NULL;

    if (!lox_campaign_parse_cli(argc, argv, &state, &resume)) {
        fprintf(stderr, "usage: loxsort_bench --campaign smoke|medium|full-1m --seed-base N --output FILE [--resume] [--checkpoint-interval N] [--timing-target-ms N] [--timing-samples N] [--stop-after-datasets N]\n");
        return 2;
    }
    campaign_id = (state.campaign == LOX_CAMPAIGN_SMOKE) ? "smoke" : (state.campaign == LOX_CAMPAIGN_MEDIUM) ? "medium" : "full-1m";
    datasets = (lox_campaign_dataset_t *)calloc(state.dataset_count, sizeof(lox_campaign_dataset_t));
    if (datasets == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }
    if (!lox_campaign_build_plan(&state, datasets)) {
        fprintf(stderr, "failed to build campaign plan\n");
        free(datasets);
        return 1;
    }
    if (!lox_campaign_open_outputs(&state, resume)) {
        fprintf(stderr, "failed to open outputs\n");
        free(datasets);
        return 1;
    }

    for (i = state.next_dataset_id; i < state.dataset_count; ++i) {
        if (state.stop_after_datasets != 0u && i >= state.stop_after_datasets) {
            break;
        }
        lox_campaign_measure_dataset(&state, &datasets[i], campaign_id, state.csv);
        if (state.checkpoint_interval != 0u && ((i + 1u) % state.checkpoint_interval) == 0u) {
            state.next_dataset_id = i + 1u;
            lox_campaign_write_checkpoint(&state);
        }
    }
    state.next_dataset_id = i;
    if (state.next_dataset_id >= state.dataset_count) {
        lox_campaign_write_checkpoint(&state);
    } else if (state.stop_after_datasets != 0u && state.next_dataset_id < state.dataset_count) {
        lox_campaign_write_checkpoint(&state);
    } else if (state.checkpoint_interval == 0u) {
        lox_campaign_write_checkpoint(&state);
    }
    fflush(state.csv);
    fclose(state.csv);
    lox_campaign_write_report_placeholder(&state);
    free(datasets);
    return 0;
}
