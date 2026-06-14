# EXEC 03 — Feature Extraction and Dispatcher

## Objective

Dokonči verejné `lox_sort()` ako deterministický constraint-aware dispatcher.

## Required reading

- `docs/API_CONTRACT.md`
- `docs/ARCHITECTURE.md`
- `docs/ALGORITHM_POLICY.md`
- `docs/TEST_STRATEGY.md`

## Execute

1. Implementuj argument validation presne podľa API kontraktu.
2. Implementuj feature extraction:
   - iba susedné páry;
   - rovnomerné indexy podľa master spec;
   - maximálne 16 comparator calls;
   - disorder a equal score;
   - žiadny sampling pre malé inputs.
3. Implementuj dispatcher v presnom poradí z `ARCHITECTURE.md`.
4. Hard constraints aplikuj pred profilovými výkonovými pravidlami.
5. Stabilný no-scratch fallback musí byť insertion sort, nie error ani nestabilný sort.
6. `PREFER_MIN_WRITES` nesmie vybrať cycle pri stability flagu alebo duplicate-heavy feature score.
7. `NO_RECURSION` nesmie nikdy vybrať introsort.
8. Opportunistic merge sa smie vybrať iba pri nenulovom profile threshold pre danú element-size class a dostatočnom scratch.
9. Invalid profile musí použiť celý generic profile a nastaviť `profile_fallback_used`.
10. `result` inicializuj pred každým error pathom.
11. Odstráň dočasný `NO_ELIGIBLE_ALGORITHM` stub z fázy 01.
12. Pridaj exact branch tests a comparator-count tests.
13. Pridaj test, ktorý porovná shared decision fixture matrix s očakávanými algoritmami a reasons.

## Forbidden

- Žiadny clock, random alebo history-dependent výber.
- Žiadne spustenie viacerých algoritmov na používateľskom vstupe.
- Žiadny full-array prescan.
- Žiadne O(sample²) feature.
- Žiadne tiché ignorovanie invalid flags.

## Required verification

- strict build;
- CTest 100 %;
- ASan + UBSan;
- build matrix intro OFF, merge OFF, cycle ON/OFF;
- `git diff --check`.

Použi povinný finálny formát z `00_INDEX.md`.
