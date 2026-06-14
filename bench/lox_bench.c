#include "lox_bench.h"
#include "lox_generators.h"
#include "lox_clock.h"

#include "../src/lox_internal.h"

#include <stdio.h>
#include <string.h>

typedef struct lox_bench_scenario_s {
    size_t count;
    size_t element_size;
} lox_bench_scenario_t;

static const lox_bench_scenario_t g_scenarios[] = {
    { 0u, 1u }, { 1u, 1u }, { 2u, 1u }, { 3u, 1u }, { 4u, 1u },
    { 7u, 2u }, { 8u, 2u }, { 15u, 4u }, { 16u, 4u }, { 23u, 8u },
    { 24u, 8u }, { 31u, 16u }, { 32u, 16u }, { 47u, 32u }, { 48u, 32u },
    { 63u, 32u }, { 64u, 32u }
};

static const lox_bench_pattern_t g_patterns[] = {
    LOX_BENCH_PATTERN_RANDOM,
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
};

static const uint32_t g_dataset_seeds[] = { 1u, 7u, 13u, 29u };
static const size_t g_timing_repetitions = 3u;

const char *lox_bench_pattern_name(lox_bench_pattern_t pattern)
{
    switch (pattern) {
    case LOX_BENCH_PATTERN_RANDOM:
        return "random";
    case LOX_BENCH_PATTERN_SORTED:
        return "sorted";
    case LOX_BENCH_PATTERN_REVERSE:
        return "reverse";
    case LOX_BENCH_PATTERN_NEARLY_SORTED:
        return "nearly_sorted";
    case LOX_BENCH_PATTERN_FEW_UNIQUE_2:
        return "few_unique_2";
    case LOX_BENCH_PATTERN_FEW_UNIQUE_8:
        return "few_unique_8";
    case LOX_BENCH_PATTERN_ALL_EQUAL:
        return "all_equal";
    case LOX_BENCH_PATTERN_ORGAN_PIPE:
        return "organ_pipe";
    case LOX_BENCH_PATTERN_SAWTOOTH:
        return "sawtooth";
    case LOX_BENCH_PATTERN_SHUFFLED_RUNS:
        return "shuffled_runs";
    case LOX_BENCH_PATTERN_ADVERSARIAL:
        return "adversarial";
    default:
        return "unknown";
    }
}

static int lox_bench_compare_raw_key(const void *lhs, const void *rhs, void *user)
{
    const unsigned char *a = (const unsigned char *)lhs;
    const unsigned char *b = (const unsigned char *)rhs;
    size_t key_size = *(const size_t *)user;
    size_t i;

    for (i = 0u; i < key_size; ++i) {
        if (a[i] < b[i]) {
            return -1;
        }
        if (a[i] > b[i]) {
            return 1;
        }
    }
    return 0;
}

static int lox_bench_is_sorted(const unsigned char *records, size_t count, size_t element_size, size_t key_size)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        if (lox_bench_compare_raw_key(
                records + ((i - 1u) * element_size),
                records + (i * element_size),
                &key_size) > 0) {
            return 0;
        }
    }
    return 1;
}

static void lox_bench_run_one(
    FILE *out,
    const lox_algorithm_entry_t *entry,
    const unsigned char *master,
    size_t count,
    size_t element_size,
    lox_bench_pattern_t pattern,
    uint32_t seed)
{
    unsigned char work[64 * 32];
    lox_sort_options_t options;
    const lox_clock_tick_source_t *clock = lox_clock_default();
    uint64_t start;
    uint64_t end;
    size_t rep;
    unsigned char scratch[4096];
    lox_status_t status = LOX_STATUS_OK;
    size_t key_size = element_size < sizeof(uint32_t) ? element_size : sizeof(uint32_t);

    if (count * element_size > sizeof(work)) {
        return;
    }

    options.scratch = NULL;
    options.scratch_size = 0u;
    options.profile = &lox_profile_generic;
    options.flags = 0u;

    if (entry->id == LOX_ALGORITHM_MERGE) {
        size_t required = lox_merge_scratch_required(count, element_size);
        if (required == SIZE_MAX || required > sizeof(scratch)) {
            return;
        }
        options.scratch = scratch;
        options.scratch_size = required;
    }

    memcpy(work, master, count * element_size);
    entry->sort(work, count, element_size, lox_bench_compare_raw_key, &key_size, &options);

    for (rep = 0u; rep < g_timing_repetitions; ++rep) {
        memcpy(work, master, count * element_size);
        start = clock->now_ticks();
        entry->sort(work, count, element_size, lox_bench_compare_raw_key, &key_size, &options);
        end = clock->now_ticks();
        status = lox_bench_is_sorted(work, count, element_size, key_size) ? LOX_STATUS_OK : LOX_STATUS_VERIFY_FAILED;
        fprintf(out,
            "%s,%s,%zu,%zu,%u,%zu,%llu,%s\n",
            lox_algorithm_name(entry->id),
            lox_bench_pattern_name(pattern),
            count,
            element_size,
            seed,
            rep,
            (unsigned long long)(end - start),
            lox_status_name(status));
    }
}

void lox_bench_run_demo(void)
{
    unsigned char master[64 * 32];
    size_t scenario_index;
    size_t pattern_index;
    size_t seed_index;
    size_t entry_count;
    const lox_algorithm_entry_t *entries = lox_registry_entries(&entry_count);

    printf("algorithm,pattern,count,element_size,dataset_seed,timing_repetition,elapsed_ticks,correctness\n");

    for (scenario_index = 0u; scenario_index < sizeof(g_scenarios) / sizeof(g_scenarios[0]); ++scenario_index) {
        const lox_bench_scenario_t *scenario = &g_scenarios[scenario_index];
        if (scenario->count * scenario->element_size > sizeof(master)) {
            continue;
        }
        for (pattern_index = 0u; pattern_index < sizeof(g_patterns) / sizeof(g_patterns[0]); ++pattern_index) {
            for (seed_index = 0u; seed_index < sizeof(g_dataset_seeds) / sizeof(g_dataset_seeds[0]); ++seed_index) {
                size_t entry_index;
                lox_bench_generate_bytes(
                    master,
                    scenario->count,
                    scenario->element_size,
                    g_dataset_seeds[seed_index],
                    g_patterns[pattern_index]);
                for (entry_index = 0u; entry_index < entry_count; ++entry_index) {
                    lox_bench_run_one(
                        stdout,
                        &entries[entry_index],
                        master,
                        scenario->count,
                        scenario->element_size,
                        g_patterns[pattern_index],
                        g_dataset_seeds[seed_index]);
                }
            }
        }
    }
}

