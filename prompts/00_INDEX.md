# LoxSort — Implementation Prompt Pack

Pred každou fázou musí agent prečítať:

- `docs/LOXSORT_SPEC.md`;
- dokumenty uvedené v konkrétnom prompte;
- `PROJECT_STATE.md`;
- aktuálny stav repozitára.

## Poradie

1. `01_foundation_api.md`
2. `02_algorithms.md`
3. `03_dispatcher_features.md`
4. `04_benchmark_profiles.md`
5. `05_tests_fuzz.md`
6. `06_build_ci_release.md`

Fázy sa nesmú preskočiť. Každá ďalšia fáza musí najprv overiť, že predchádzajúci baseline reálne builduje a testy prechádzajú.

## Globálne exekučné pravidlá

- Vykonaj iba požadovanú fázu.
- Nepridávaj nové algoritmy, API, runtime learning, plugin ABI ani feature extraction nad rámec špecifikácie.
- Žiadne placeholdery, no-op implementácie alebo úspešné returny bez vykonania sľúbenej práce.
- Žiadne TODO/FIXME v produkčnom kóde.
- Žiadne `malloc`, `calloc`, `realloc`, `free`, VLA, GCC attributes ani linker sections.
- Zachovaj ISO C99.
- Neupravuj nesúvisiace súbory.
- Neuvádzaj agenta v git commite ako Author, CoAuthor ani cez `Co-authored-by` trailer.
- Nevytváraj commit, ak povinné testy alebo build neprešli.
- Tvrdenie „verified“ je povolené iba s priloženým príkazom a výsledkom.
- Neuvádzaj roadmap, optimistické zhrnutia ani marketingové tvrdenia.

## Povinný finálny formát každého agenta

```text
CHANGED FILES
- ...

CODE CHANGES
- ...

TEST EVIDENCE
- command: ...
- result: ...

BUILD EVIDENCE
- command: ...
- result: ...

VERIFIED FACTS
- ...

NOT VERIFIED
- ...

INCOMPLETE
- ...
```

Sekcie musia existovať presne v tomto poradí. Prázdna sekcia obsahuje `- None`.
