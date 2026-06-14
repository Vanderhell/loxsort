# EXEC 01 — Foundation, Public API and Internal Contracts

## Objective

Vytvor kompilovateľný základ LoxSortu bez implementácie sort algoritmov a bez predstierania funkčného `lox_sort()`.

## Required reading

- `docs/LOXSORT_SPEC.md`
- `docs/API_CONTRACT.md`
- `docs/ARCHITECTURE.md`
- `docs/FILE_LAYOUT.md`
- `docs/DECISIONS.md`

## Execute

1. Inicializuj cieľovú adresárovú štruktúru podľa `FILE_LAYOUT.md`.
2. Vytvor verejný `include/loxsort/loxsort.h` presne podľa `API_CONTRACT.md`.
3. Vytvor interné typy pre:
   - feature vector;
   - algorithm function pointer;
   - explicit registry entry;
   - compiled algorithm mask;
   - profile validation result.
4. Vytvor `src/lox_profile.c` s:
   - `lox_profile_generic`;
   - úplnou validáciou profilu;
   - whole-profile fallbackom, nie čiastočným opravovaním vstupného profilu.
5. Vytvor `lox_merge_scratch_required()` s overflow detekciou.
6. Vytvor string helpery `lox_algorithm_name()` a `lox_status_name()`.
7. Vytvor bezpečné internal utility funkcie:
   - checked multiplication;
   - element address calculation po predchádzajúcej validácii;
   - generic swap bez VLA a heapu;
   - byte-copy helper, iba ak je reálne potrebný.
8. `lox_sort()` v tejto fáze nesmie klamať. Môže korektne obslúžiť iba `count < 2`; pre `count >= 2` dočasne vráti `LOX_STATUS_NO_ELIGIBLE_ALGORITHM`. Tento dočasný stav musí byť odstránený vo fáze 03.
9. Vytvor minimálne testy public headeru, profile validatora, overflow a trivial inputu.
10. Vytvor minimálny CMake target knižnice a CTest, iba v rozsahu potrebnom na build tejto fázy.

## Forbidden

- Žiadny opaque context.
- Žiadny algorithm implementation okrem trivial input path.
- Žiadny benchmark code.
- Žiadny runtime global default state.
- Žiadne public plugin API.

## Required verification

- clean configure;
- strict C99 build s warnings as errors;
- CTest;
- `git diff --check`.

Použi povinný finálny formát z `00_INDEX.md`.
