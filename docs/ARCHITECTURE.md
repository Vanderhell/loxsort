# LoxSort — Architecture

## Vrstvy

```text
USER
  |
  v
PUBLIC API
  - validácia
  - options defaults
  - result initialization
  |
  v
DISPATCHER
  - compiled algorithm mask
  - profile validation
  - hard constraints
  - optional fixed-cost sampling
  - deterministic selection
  |
  v
EXPLICIT REGISTRY
  - compile-time entries
  - no linker sections
  |
  v
ALGORITHMS
  - insertion
  - shell
  - introsort
  - merge
  - optional cycle
```

Benchmark a profile generator sú mimo runtime knižnice:

```text
DATA GENERATORS -> BENCHMARK RUNNER -> CSV -> PROFILE GENERATOR
                                               |
                                               v
                                 generated/lox_profile_target.h
```

## Runtime komponenty

### Public API

Zodpovedá iba za bezpečný kontrakt. Neobsahuje platformový clock, filesystem, benchmark ani heap.

### Profile validator

Overí:

- `format_version`;
- `sample_pairs` v rozsahu 1–16;
- povinný insertion bit;
- prahy v platných rozsahoch;
- algoritmickú masku voči skompilovaným algoritmom.

Neplatný profil nespôsobí pád ani čiastočné použitie. Celý sa nahradí generic profilom.

### Feature extractor

- read-only prístup k používateľskému poľu;
- najviac 16 comparator callov;
- žiadne pointer arithmetic mimo overeného rozsahu;
- žiadny persistentný stav.

### Dispatcher

Dispatcher nie je ML classifier. Je to malý policy engine parametrizovaný profilom. Constraints vždy filtrujú oprávnené algoritmy skôr než sa použijú výkonové prahy.

### Registry

Registry je explicitné statické pole zostavené pomocou `#if LOXSORT_ENABLE_*`. Algoritmus nie je automaticky objavený linkerom.

Každý registry entry obsahuje minimálne:

```c
typedef struct lox_algorithm_entry_s {
    lox_algorithm_t id;
    uint16_t mask_bit;
    uint8_t is_stable;
    uint8_t uses_recursion;
    lox_algorithm_fn sort;
} lox_algorithm_entry_t;
```

Registry môže byť interný. Verejný plugin ABI nie je cieľ v0.1.

## Žiadny opaque context

Pôvodný návrh kombinoval neúplný `lox_ctx_t` s používateľskou stack alokáciou. To je neplatný C kontrakt. v0.1 preto nemá context lifecycle. Stateless API je menšie, jednoduchšie a reentrantné.

## Rozhodovací pseudokód

```text
validate arguments
initialize result

if count < 2:
    return OK

profile = validate_or_generic(options.profile)
mask = compiled_mask & profile.algorithm_mask
mask |= INSERTION

if count <= profile.small_count_cutoff:
    run INSERTION

if REQUIRE_STABLE:
    if MERGE eligible and scratch is sufficient:
        sample
        if count <= stable_insertion_cutoff and nearly_sorted:
            run INSERTION
        run MERGE
    run INSERTION as stable no-scratch fallback

sample only if remaining branches need it

if PREFER_MIN_WRITES and CYCLE eligible:
    if count >= cycle_min_count and equal_score <= cycle_equal_score_max:
        run CYCLE

if nearly_sorted and count <= near_sorted_insertion_max_count:
    run INSERTION

if NO_RECURSION:
    run SHELL if eligible, otherwise INSERTION

merge_min = profile merge threshold for element size class
if MERGE eligible and scratch is sufficient and merge_min != 0 and count >= merge_min:
    run MERGE

shell_limit = profile limit for element size class
if SHELL eligible and count <= shell_limit:
    run SHELL

if INTRO eligible:
    run INTRO

if SHELL eligible:
    run SHELL

run INSERTION
```

## Determinizmus

Pre rovnaké:

- pole;
- comparator;
- options;
- profil;
- build konfiguráciu;

musí dispatcher vybrať rovnaký algoritmus. Clock, random seed ani globálna história nesmú ovplyvniť runtime rozhodnutie.

## Error containment

- Argument error: žiadna modifikácia vstupu.
- Invalid profile: generic fallback, nie error.
- Nedostatočný scratch: merge sa vyradí; stabilita sa zachová insertion fallbackom.
- Chýbajúci voliteľný algoritmus: dispatcher zvolí ďalší oprávnený fallback.
- Verify failure: `LOX_STATUS_VERIFY_FAILED`; pole ostáva v stave po algoritme, pretože rollback bez ďalšej plnej kópie nie je garantovaný.
