# LoxSort â€” Project State

## Current phase

**IMPLEMENTED BASELINE / LOCAL VALIDATION COMPLETE**

## Verified design decisions

- LoxSort je constraint-aware deterministic dispatcher, nie runtime samoučiaci engine.
- Runtime nikdy nespúšťa viac algoritmov na používateľských dátach pre výber víťaza.
- Benchmarking je oddelený od runtime knižnice.
- Malé polia idú priamo na insertion sort bez feature extraction.
- Feature extraction používa najviac 16 susedných porovnaní.
- Verejné API je stateless a nevyžaduje context object.
- Povinný algoritmus je insertion sort.
- Katalóg obsahuje insertion, Shell, introsort a merge sort; cycle je voliteľný.
- Heapsort je interný fallback introsortu.
- Registry je explicitný compile-time zoznam.
- `lox_calibrate()` nie je súčasť v0.1.

## Local validation

- Debug configure/build/test: verified.
- Release configure/build/test: verified.
- Benchmark target build: verified.
- Install/export package: verified.
- External consumer build/run: verified.
- Clang sanitizer build/test: verified with local LLVM runtime path.

## Implementation status

- Public API: implemented
- Algorithms: implemented
- Dispatcher: implemented
- Benchmark tools: implemented at skeleton level
- Profile generator: implemented at skeleton level
- Tests: implemented
- Sanitizer build support: implemented
- CI workflow file: present
- Fuzz targets: not implemented

