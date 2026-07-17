# LoxSort — Master Specification

**Family:** Liquid Oxygen  
**Version:** v1.0.0  
**Status:** záväzný návrh pred implementáciou  
**Language:** ISO C99  
**Target:** embedded a resource-constrained systémy

---

## 1. Produktová definícia

LoxSort je malá stateless knižnica, ktorá zoradí používateľské pole a automaticky zvolí najvhodnejší **povolený** algoritmus podľa:

- počtu prvkov;
- veľkosti jedného prvku;
- lacnej vzorky lokálneho usporiadania dát;
- požiadavky na stabilitu;
- dostupného scratch buffra;
- zákazu rekurzie;
- preferencie minimálneho počtu zápisov;
- statického benchmark profilu konkrétnej platformy.

LoxSort negarantuje abstraktne „globálne najrýchlejší algoritmus“. Garantuje korektné triedenie a deterministický výber podľa známych obmedzení a nameraného profilu.

## 2. Priority

Poradie priorít je záväzné:

1. **Korektnosť** — výstup je zoradený a obsahuje presne pôvodné prvky.
2. **Splnenie tvrdých obmedzení** — stabilita, zákaz rekurzie, platná pamäť.
3. **Predvídateľnosť** — bez undefined behavior, skrytých alokácií a skrytého mutable stavu.
4. **Nízka runtime réžia** — malé vstupy bez analýzy, maximálne 16 vzorkovacích porovnaní.
5. **Výkon podľa profilu** — výber medzi oprávnenými algoritmami na základe benchmarkov.
6. **Malý footprint** — voliteľné algoritmy sa dajú vypnúť pri kompilácii.

## 3. Non-goals pre v0.1

LoxSort v0.1 nie je:

- runtime samoučiaci systém;
- runtime kalibrátor;
- paralelný alebo externý sort;
- databázový sort;
- C++ template knižnica;
- plugin systém založený na linker sections;
- platformovo univerzálny výkonový profil;
- záruka konštantného času.

`lox_calibrate()` neexistuje. Ak bude runtime kalibrácia niekedy pridaná, musí ísť o samostatnú neskoršiu capability s jasným persistence a memory kontraktom.

## 4. Algoritmy v0.1

### 4.1 Povinné

- **Insertion sort** — stabilný, bez extra pamäte, malé alebo skoro zoradené vstupy.
- **Shell sort** — bez extra pamäte a bez rekurzie, stredné všeobecné vstupy.
- **Introsort** — všeobecná rýchla cesta; quick partitioning, bounded recursion, interný heapsort fallback a insertion finish.
- **Bottom-up merge sort** — stabilná cesta pri dostupnom scratch buffri.

### 4.2 Voliteľné

- **Cycle sort** — špecializovaná nestabilná cesta pre preferenciu minimálnych zápisov. Predvolene je vypnutý a musí mať samostatné testy pre duplicity.

### 4.3 Výslovne odstránené

- Timsort;
- selection sort;
- comb sort;
- samostatný quicksort;
- samostatný verejný heapsort.

Heapsort zostáva internou súčasťou introsortu.

## 5. Verejný model

Knižnica nemá init/deinit lifecycle. Každé volanie dostane všetko potrebné cez argumenty a voliteľnú `lox_sort_options_t`.

```c
lox_status_t lox_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options,
    lox_sort_result_t *result);
```

- `options == NULL` použije bezpečné defaults a generic profil.
- `result == NULL` je povolené.
- Knižnica nevlastní používateľské dáta ani scratch buffer.
- Knižnica nesmie uložiť pointery z volania do globálneho alebo statického mutable stavu.

Presný kontrakt je v `API_CONTRACT.md`.

## 6. Rozhodovací tok

Dispatcher musí vykonať kroky v tomto poradí:

1. Validácia argumentov a aritmetického pretečenia.
2. `count < 2` → úspešný návrat bez porovnania.
3. `count <= profile.small_count_cutoff` → insertion sort bez feature extraction.
4. Vytvorenie masky algoritmov skompilovaných do buildu a povolených profilom.
5. Vyhodnotenie tvrdých obmedzení.
6. Feature extraction iba vtedy, keď môže zmeniť rozhodnutie.
7. Výber algoritmu.
8. Spustenie presne jedného algoritmu.
9. Voliteľná post-sort verifikácia.
10. Vyplnenie `lox_sort_result_t`.

Runtime nikdy nespustí viac sortov na rovnakých používateľských dátach s cieľom vybrať víťaza.

## 7. Feature extraction

### 7.1 Rozsah

Feature extraction je pevne ohraničená a analyzuje najviac `profile.sample_pairs` susedných dvojíc, pričom:

```text
1 <= sample_pairs <= 16
pair_count = min(sample_pairs, count - 1)
```

Pre vzorku `s` v intervale `[0, pair_count)`:

```text
left = floor(s * (count - 1) / pair_count)
right = left + 1
```

Z toho vyplýva:

```text
0 <= left < count - 1
1 <= right < count
```

Nesmie existovať inkluzívna slučka `0..pair_count`.

### 7.2 Výstup

Feature vector obsahuje iba:

- `count_bucket`;
- `element_size_class`;
- `disorder_score` v rozsahu 0–255;
- `equal_score` v rozsahu 0–255;
- počet reálne vykonaných vzoriek.

```text
disorder_score = descending_pairs * 255 / pair_count
equal_score    = equal_pairs      * 255 / pair_count
```

Pri `pair_count == 0` sa feature extraction nevolá.

Žiadne O(sample²) zisťovanie unikátnosti, tail probe ani úplný lineárny prescan nie sú súčasťou v0.1.

### 7.3 Rozpočet

- malé vstupy: 0 feature porovnaní;
- ostatné vstupy: maximálne 16 feature porovnaní;
- žiadna dynamická alokácia;
- žiadny zápis do triedeného poľa.

## 8. Rozhodovacia politika

Presné pravidlá sú v `ALGORITHM_POLICY.md`. Základná hierarchia je:

```text
malý vstup
  -> insertion

stabilita požadovaná
  -> insertion pre lacný skoro zoradený prípad
  -> inak merge, ak je dostupný plný scratch
  -> inak stabilný insertion fallback

preferencia minimálnych zápisov
  -> cycle, iba ak je skompilovaný, profilom povolený a vzorka nie je duplicate-heavy

zákaz rekurzie
  -> insertion pre skoro zoradené dáta
  -> inak Shell

bežný vstup
  -> insertion pre profilom ohraničené skoro zoradené dáta
  -> merge, ak profil pre danú veľkosť elementu preukázal výhodu a scratch stačí
  -> Shell do profilového limitu
  -> introsort pre väčšie dáta
```

Benchmark profil upravuje prahy. Nesmie obísť tvrdé obmedzenia.

## 9. Profily

### 9.1 Generic profil

Repozitár musí obsahovať konzervatívny `lox_profile_generic`, ktorý funguje bez benchmarkovania cieľového hardvéru.

### 9.2 Platformový profil

Offline benchmark môže vytvoriť profil viazaný na:

- target/CPU;
- compiler a verziu;
- optimalizačné flags;
- veľkostný model platformy;
- zostavu povolených algoritmov;
- verziu formátu profilu.

Profil nesmie byť automaticky označený ako univerzálny. Profil z x86 hosta nie je autoritatívny pre ESP32, STM32 alebo AVR.

### 9.3 Validácia

Ak profil nemá podporovanú verziu alebo odkazuje na algoritmus, ktorý nie je skompilovaný, dispatcher použije generic profil a uvedie fallback v `lox_sort_result_t`.

## 10. Pamäťový model

- Žiadne volanie heap API.
- Žiadne VLA (variable length array).
- Swapy musia fungovať po blokoch s malým fixným lokálnym bufferom alebo byte-wise fallbackom.
- Merge sort vyžaduje scratch aspoň `count * element_size` bajtov.
- Výpočet požadovanej veľkosti musí detegovať `size_t` overflow pred násobením.
- Introsort má bounded recursion `O(log count)` a heapsort fallback.
- `LOX_SORT_NO_RECURSION` introsort úplne vyradí.

## 11. Portabilita

- Jazyk: ISO C99.
- Zakázané: GCC attributes, linker sections, C11 atomics, threads, TLS, VLAs a nested functions.
- Povolené: `stdint.h`, `stddef.h`, `string.h`, `limits.h`.
- Knižničná časť nesmie vyžadovať OS, clock ani filesystem.
- Benchmark nástroje môžu byť host-only a mať samostatnú platform abstraction pre clock.

## 12. Benchmarkovanie

Benchmarky sú samostatná host/target vrstva. Musia:

- generovať dataset mimo timing intervalu;
- vytvoriť identickú pracovnú kópiu pre každý algoritmus;
- oddeliť `dataset_seed` od `timing_repetition`;
- merať rovnaký dataset opakovane;
- verifikovať výsledok po každom meraní;
- reportovať median a percentily, nie iba minimum;
- reportovať comparisons, moved bytes a writes, ak je instrumentácia zapnutá;
- nepoužívať výsledky chybného alebo nezoradeného behu.

Profil generator nepoužíva ML framework. Deterministicky prehľadá definovaný priestor prahov a zvolí profil s najnižším validovaným regretom pri rozumnej penalizácii code/runtime complexity.

## 13. Testovanie

Minimálny testovací rozsah:

- exhaustive malé polia;
- náhodné a štruktúrované vzory;
- všetky podporované veľkosti elementov;
- duplicity;
- stabilita tagged records;
- argument validation a overflow;
- dispatcher branch coverage;
- profile fallback;
- sanitizéry;
- fuzzing;
- čistý CMake/CTest build.

Detaily sú v `TEST_STRATEGY.md`.

## 14. Build-time možnosti

Minimálne možnosti:

```text
LOXSORT_ENABLE_SHELL
LOXSORT_ENABLE_INTRO
LOXSORT_ENABLE_MERGE
LOXSORT_ENABLE_CYCLE
LOXSORT_ENABLE_VERIFY
LOXSORT_ENABLE_INSTRUMENTATION
LOXSORT_BUILD_TESTS
LOXSORT_BUILD_BENCH
```

Insertion sort sa nedá vypnúť, pretože je bezpečný povinný fallback.

## 15. Definition of Done pre v0.1

v0.1 je hotová iba vtedy, keď:

- verejné API zodpovedá špecifikácii;
- všetky algoritmy sú korektné a otestované;
- dispatcher rešpektuje constraints;
- malé vstupy neplatia analytickú réžiu;
- feature budget je testom dokázaný;
- generic profil existuje;
- platformový profil sa dá deterministicky vygenerovať;
- ASan, UBSan a fuzz baseline sú bez nálezu;
- release checklist je splnený;
- dokumentácia neobsahuje tvrdenia, ktoré testy alebo build nepotvrdili.
