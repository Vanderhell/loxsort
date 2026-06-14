# EXEC 02 — Sorting Algorithms

## Objective

Implementuj a priamo otestuj algoritmy bez dispatcher logiky.

## Required reading

- `docs/LOXSORT_SPEC.md`
- `docs/ALGORITHM_POLICY.md`
- `docs/TEST_STRATEGY.md`
- existujúce internal contracts z fázy 01

## Execute

1. Implementuj stabilný insertion sort.
2. Implementuj Shell sort s jednou presne zdokumentovanou Ciura gap sequence.
3. Implementuj introsort:
   - bounded recursion;
   - depth limit `2 * floor(log2(n))`;
   - internal heapsort fallback;
   - duplicate-safe partitioning;
   - insertion finish pre malé partitions;
   - bez VLA.
4. Implementuj bottom-up stabilný merge sort s plným caller-owned scratch.
5. Implementuj cycle sort iba pod `LOXSORT_ENABLE_CYCLE` a default option nech zostane OFF.
6. Vytvor explicitný registry cez compile-time `#if`, bez linker magic.
7. Každý algoritmus musí mať priame tests:
   - edge cases;
   - structured patterns;
   - random deterministic cases;
   - non-standard element sizes;
   - permutation;
   - stability, kde platí;
   - insufficient scratch pre merge;
   - duplicate-heavy cycle cases.
8. Testuj build varianty cycle OFF/ON, merge OFF/ON a intro OFF.
9. Produkčný kód nesmie obsahovať test-only globálne counters.

## Forbidden

- Žiadny dispatcher.
- Žiadne feature extraction.
- Žiadne benchmark-driven tvrdenia.
- Žiadny samostatný public heapsort alebo quicksort.
- Žiadny Timsort, selection alebo comb sort.

## Required verification

- strict GCC/Clang build, ak sú dostupné;
- CTest 100 %;
- ASan + UBSan pre algorithm tests;
- `git diff --check`.

Použi povinný finálny formát z `00_INDEX.md`.
