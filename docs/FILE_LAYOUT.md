# LoxSort — Target Repository Layout

```text
loxsort/
├─ CMakeLists.txt
├─ LICENSE
├─ README.md
├─ PROJECT_STATE.md
├─ RELEASE_CHECKLIST.md
├─ cmake/
│  └─ LoxSortWarnings.cmake
├─ include/
│  └─ loxsort/
│     └─ loxsort.h
├─ src/
│  ├─ lox_api.c
│  ├─ lox_dispatch.c
│  ├─ lox_features.c
│  ├─ lox_profile.c
│  ├─ lox_registry.c
│  ├─ lox_internal.h
│  ├─ lox_swap.c
│  └─ algorithms/
│     ├─ lox_insertion.c
│     ├─ lox_shell.c
│     ├─ lox_intro.c
│     ├─ lox_heap_internal.c
│     ├─ lox_merge.c
│     └─ lox_cycle.c
├─ generated/
│  └─ lox_profile_generic_generated.h
├─ bench/
│  ├─ lox_bench_main.c
│  ├─ lox_bench.c
│  ├─ lox_bench.h
│  ├─ lox_generators.c
│  ├─ lox_generators.h
│  ├─ lox_clock.h
│  ├─ lox_clock_posix.c
│  └─ lox_clock_windows.c
├─ tools/
│  ├─ generate_profile.py
│  └─ validate_benchmark.py
├─ tests/
│  ├─ test_main.c
│  ├─ test_api.c
│  ├─ test_features.c
│  ├─ test_dispatch.c
│  ├─ test_profile.c
│  ├─ test_algorithms.c
│  ├─ test_stability.c
│  ├─ test_generators.c
│  ├─ fixtures/
│  └─ fuzz/
│     ├─ fuzz_public_api.c
│     ├─ fuzz_algorithms.c
│     └─ corpus/
├─ docs/
│  ├─ LOXSORT_SPEC.md
│  ├─ API_CONTRACT.md
│  ├─ ARCHITECTURE.md
│  ├─ ALGORITHM_POLICY.md
│  ├─ BENCHMARK_AND_PROFILES.md
│  ├─ TEST_STRATEGY.md
│  ├─ FILE_LAYOUT.md
│  └─ DECISIONS.md
└─ .github/
   └─ workflows/
      ├─ ci.yml
      └─ fuzz-smoke.yml
```

## Rules

- Public declarations sú iba v `include/loxsort/loxsort.h`.
- Interné headers nie sú inštalované.
- Benchmark sources nie sú súčasťou `loxsort` library targetu.
- Test instrumentation nie je v produkčnom ABI.
- Generated target profiles sa nepridávajú bez reportu o prostredí a quality gates.
- `generated/lox_profile_generic_generated.h` môže byť generovaný zo source-of-truth fixture, ale build z release tarballu nesmie vyžadovať Python.
