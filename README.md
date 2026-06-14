# LoxSort

LoxSort je malá deterministická C99 knižnica pre embedded a resource-constrained systémy. Vyberá triediaci algoritmus podľa veľkosti vstupu, stability, scratch buffra, zákazu rekurzie a offline profilu.

## Stav

V tomto repozitári je implementovaný:

- verejný header `include/loxsort/loxsort.h`;
- insertion, Shell, introsort a bottom-up merge sort;
- voliteľný cycle sort;
- deterministický dispatcher a profilová validácia;
- benchmark skeleton, profile generator a CMake export/install;
- test suite s API, algoritmami, dispatchom, profilom a stabilitou.

## Build

```text
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

### Užitočné CMake voľby

- `LOXSORT_BUILD_TESTS`
- `LOXSORT_BUILD_BENCH`
- `LOXSORT_ENABLE_CYCLE`
- `LOXSORT_ENABLE_VERIFY`
- `LOXSORT_ENABLE_SANITIZERS`

## Exportovaný balík

Projekt inštaluje:

- `include/loxsort/loxsort.h`;
- `loxsort::loxsort` CMake target;
- `loxsortConfig.cmake` pre `find_package(loxsort CONFIG REQUIRED)`.

## Dokumentácia

- `docs/LOXSORT_SPEC.md`
- `docs/API_CONTRACT.md`
- `docs/ARCHITECTURE.md`
- `docs/ALGORITHM_POLICY.md`
- `docs/BENCHMARK_AND_PROFILES.md`
- `docs/TEST_STRATEGY.md`
- `docs/FILE_LAYOUT.md`
- `docs/DECISIONS.md`

