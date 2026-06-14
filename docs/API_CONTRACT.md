# LoxSort — Public API Contract

Tento dokument definuje záväzný verejný povrch v0.1. Názvy sa počas implementácie nemajú meniť bez aktualizácie master špecifikácie.

## Header

Verejný header je jediný súbor:

```text
include/loxsort/loxsort.h
```

Musí byť samostatne kompilovateľný ako C99 aj includovateľný z C++ cez `extern "C"`.

## Typy

```c
#ifndef LOXSORT_LOXSORT_H
#define LOXSORT_LOXSORT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOX_SORT_VERSION_MAJOR 0u
#define LOX_SORT_VERSION_MINOR 1u
#define LOX_SORT_VERSION_PATCH 0u

#define LOX_PROFILE_FORMAT_VERSION 1u
#define LOX_PROFILE_MAX_SAMPLE_PAIRS 16u

typedef int (*lox_compare_fn)(
    const void *lhs,
    const void *rhs,
    void *user);

typedef enum lox_status_e {
    LOX_STATUS_OK = 0,
    LOX_STATUS_INVALID_ARGUMENT,
    LOX_STATUS_SIZE_OVERFLOW,
    LOX_STATUS_NO_ELIGIBLE_ALGORITHM,
    LOX_STATUS_VERIFY_FAILED
} lox_status_t;

typedef enum lox_algorithm_e {
    LOX_ALGORITHM_NONE = 0,
    LOX_ALGORITHM_INSERTION,
    LOX_ALGORITHM_SHELL,
    LOX_ALGORITHM_INTRO,
    LOX_ALGORITHM_MERGE,
    LOX_ALGORITHM_CYCLE
} lox_algorithm_t;

typedef enum lox_decision_reason_e {
    LOX_REASON_TRIVIAL_INPUT = 0,
    LOX_REASON_SMALL_INPUT,
    LOX_REASON_NEARLY_SORTED,
    LOX_REASON_STABLE_WITH_SCRATCH,
    LOX_REASON_STABLE_FALLBACK,
    LOX_REASON_MIN_WRITES,
    LOX_REASON_NO_RECURSION,
    LOX_REASON_PROFILE_MERGE_RANGE,
    LOX_REASON_PROFILE_SHELL_RANGE,
    LOX_REASON_PROFILE_GENERAL,
    LOX_REASON_PROFILE_FALLBACK
} lox_decision_reason_t;

enum {
    LOX_SORT_REQUIRE_STABLE   = 1u << 0,
    LOX_SORT_PREFER_MIN_WRITES = 1u << 1,
    LOX_SORT_NO_RECURSION     = 1u << 2,
    LOX_SORT_VERIFY_RESULT    = 1u << 3
};

enum {
    LOX_ALGORITHM_MASK_INSERTION = 1u << 0,
    LOX_ALGORITHM_MASK_SHELL     = 1u << 1,
    LOX_ALGORITHM_MASK_INTRO     = 1u << 2,
    LOX_ALGORITHM_MASK_MERGE     = 1u << 3,
    LOX_ALGORITHM_MASK_CYCLE     = 1u << 4
};

typedef struct lox_profile_s {
    uint16_t format_version;
    uint16_t algorithm_mask;

    uint16_t small_count_cutoff;
    uint16_t stable_insertion_cutoff;
    uint16_t near_sorted_insertion_max_count;
    uint16_t cycle_min_count;

    uint16_t shell_max_count_elem_small;
    uint16_t shell_max_count_elem_medium;
    uint16_t shell_max_count_elem_large;

    uint16_t merge_min_count_elem_small;
    uint16_t merge_min_count_elem_medium;
    uint16_t merge_min_count_elem_large;

    uint8_t sample_pairs;
    uint8_t near_sorted_disorder_max;
    uint8_t cycle_equal_score_max;
    uint8_t reserved0;
} lox_profile_t;

typedef struct lox_sort_options_s {
    void *scratch;
    size_t scratch_size;
    const lox_profile_t *profile;
    uint32_t flags;
} lox_sort_options_t;

typedef struct lox_sort_result_s {
    lox_algorithm_t algorithm;
    lox_decision_reason_t reason;
    uint8_t profile_fallback_used;
    uint8_t sampled_pair_count;
    uint8_t disorder_score;
    uint8_t equal_score;
} lox_sort_result_t;

extern const lox_profile_t lox_profile_generic;

lox_status_t lox_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options,
    lox_sort_result_t *result);

size_t lox_merge_scratch_required(size_t count, size_t element_size);

const char *lox_algorithm_name(lox_algorithm_t algorithm);
const char *lox_status_name(lox_status_t status);

#ifdef __cplusplus
}
#endif

#endif
```

## Argument validation

### `count == 0`

- `base` môže byť `NULL`;
- `compare` môže byť `NULL`;
- `element_size` môže byť 0;
- návrat je `LOX_STATUS_OK`;
- výsledok: `LOX_ALGORITHM_NONE`, `LOX_REASON_TRIVIAL_INPUT`.

### `count == 1`

- `base` musí byť nenulové, ak `element_size > 0`;
- `element_size` musí byť väčšie než 0;
- comparator sa nevolá;
- návrat je `LOX_STATUS_OK`.

### `count >= 2`

Povinné:

- `base != NULL`;
- `element_size > 0`;
- `compare != NULL`;
- `count * element_size` nesmie pretiecť `size_t`.

Porušenie vracia `LOX_STATUS_INVALID_ARGUMENT` alebo `LOX_STATUS_SIZE_OVERFLOW` a nesmie meniť pole.

## Comparator contract

Comparator musí:

- vrátiť zápornú hodnotu, ak `lhs < rhs`;
- vrátiť 0, ak sú prvky ekvivalentné;
- vrátiť kladnú hodnotu, ak `lhs > rhs`;
- vytvárať konzistentné strict weak ordering;
- nemať side effect, ktorý mení triedené pole;
- ak používa stav, čítať ho cez `user`.

LoxSort nesmie predpokladať presné hodnoty `-1`, `0`, `1`.

## Options semantics

`options == NULL` znamená:

- bez scratch buffra;
- generic profil;
- žiadna požadovaná stabilita;
- rekurzia povolená;
- bez preferencie minimálnych zápisov;
- verifikácia podľa build-time defaultu.

Neznáme bity v `flags` sú invalid argument. `scratch == NULL` vyžaduje `scratch_size == 0`.

### Konflikty preferencií

- `REQUIRE_STABLE` je tvrdá požiadavka.
- `NO_RECURSION` je tvrdá požiadavka.
- `PREFER_MIN_WRITES` je mäkká preferencia.
- Ak je požadovaná stabilita, cycle sort nie je oprávnený.
- Ak je zakázaná rekurzia, introsort nie je oprávnený.
- Stabilita má prednosť pred preferenciou minimálnych zápisov.

## Scratch contract

- Scratch vlastní volajúci.
- Scratch sa môže meniť.
- Scratch sa nesmie prekrývať s rozsahom triedeného poľa.
- Merge sort vyžaduje najmenej `count * element_size` bajtov.
- `lox_merge_scratch_required()` vracia `SIZE_MAX` pri overflow.

## Result contract

Ak `result != NULL`, funkcia ho inicializuje na deterministické hodnoty pred prvým možným error returnom.

Pri úspechu obsahuje:

- skutočne spustený algoritmus;
- dôvod výberu;
- či bol použitý generic profile fallback;
- počet feature porovnaní;
- feature scores alebo nuly, ak sa nevzorkovalo.

## Thread safety

Knižnica je reentrantná a thread-safe, ak:

- jednotlivé volania nepoužívajú rovnaké mutable pole alebo scratch;
- comparator user state je thread-safe;
- build nemá zapnutú globálnu debug instrumentáciu.

Výkonnostná instrumentácia nesmie byť implementovaná cez neizolované globálne počítadlá v produkčnej knižnici. Testovacia instrumentácia musí byť explicitne injektovaná alebo obmedzená na test build.
