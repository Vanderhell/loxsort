# LoxSort — Benchmark and Profile Specification

## Cieľ

Benchmark nemá dokazovať, že jeden algoritmus je univerzálne najlepší. Má vytvoriť malý profil pre konkrétnu kombináciu:

```text
target + compiler + compiler version + flags + algorithm set
```

Runtime potom používa iba prahy z profilu.

## Scenáre

### Sizes

Povinné sú hranice okolo potenciálnych dispatcher cutoffov, nie iba mocniny dvoch:

```text
0, 1, 2, 3, 4, 7, 8, 15, 16, 23, 24, 31, 32,
47, 48, 63, 64, 95, 96, 127, 128, 191, 192,
255, 256, 383, 384, 511, 512, 767, 768, 1024
```

Target runner môže rozsah znížiť podľa RAM, ale musí reportovať vynechané scenáre.

### Element sizes

Minimálne:

```text
1, 2, 4, 8, 16, 32 bajtov
```

Element musí obsahovať sortable key a payload, aby meranie väčších elementov nebolo iba porovnaním raw bytes bez realistického presunu.

### Patterns

Minimálne:

1. random uniform;
2. sorted;
3. reverse;
4. nearly sorted — 1 % swaps;
5. nearly sorted — 5 % swaps;
6. few unique — 2 hodnoty;
7. few unique — 8 hodnôt;
8. all equal;
9. organ pipe;
10. sawtooth;
11. shuffled runs;
12. adversarial partition pattern pre introsort.

Generátory sú deterministické a nealokujú.

## Dve oddelené osi opakovania

### Dataset seed

Určuje obsah poľa. Rôzne seedy predstavujú rôzne datasety v rovnakom scenári.

### Timing repetition

Opakovane meria ten istý dataset. Pred každým meraním sa pracovné pole obnoví z nezmenenej master kópie mimo timing intervalu.

Tieto osi sa nesmú zlúčiť do jedného `run_index`.

Odporúčaný default:

```text
20 dataset seeds × 15 timing repetitions
```

Host quick mode môže použiť menej, no report to musí uviesť.

## Férový measurement loop

Pre každý scenario a dataset seed:

1. Vygeneruj master dataset.
2. Pre každý algoritmus:
   1. over, že je pre scenario oprávnený;
   2. vykonaj warmup mimo reportovaných meraní;
   3. pre každú timing repetition skopíruj master do work buffra;
   4. resetuj instrumentáciu;
   5. spusti timer;
   6. spusti algoritmus;
   7. zastav timer;
   8. verifikuj zoradenie a permutáciu;
   9. zapíš výsledok.

Generovanie, obnova work buffra a verifikácia nie sú súčasťou measured ticks.

## Metrics

Povinné:

- elapsed ticks a tick frequency;
- algorithm;
- pattern;
- count;
- element size;
- dataset seed;
- timing repetition;
- correctness status.

Odporúčané instrumentované buildy:

- comparator calls;
- swap count;
- moved bytes;
- logical writes;
- maximum recursion depth;
- scratch bytes.

Instrumentované a timing výsledky sa nesmú miešať ako keby pochádzali z rovnakého výkonového buildu.

## Summary

Pre každý algorithm/scenario/dataset sa vypočíta minimálne:

- median;
- p10 a p90;
- median absolute deviation;
- failures.

Agregácia cez dataset seeds reportuje median a p95 regret.

## Regret

Pre scenár `s`:

```text
regret(selected, s) = median_time(selected, s) / median_time(best_eligible, s)
```

Chybný algoritmus nie je eligible. Hard constraints sa aplikujú pred výpočtom víťaza.

## Profile generator

`tools/generate_profile.py` používa iba Python standard library. Žiadny scikit-learn ani runtime ML model.

Generator:

1. načíta raw alebo validated summary CSV;
2. odmietne nekompletné a chybné scenáre;
3. prehľadá explicitný grid kandidátnych prahov;
4. simuluje runtime dispatcher;
5. vyhodnotí median, p90, p95 a maximum regret;
6. použije deterministic tie-break:
   1. nižší p95 regret;
   2. nižší median regret;
   3. nižší maximum regret;
   4. menší sampling budget;
   5. konzervatívnejší generic-like profil;
7. vygeneruje C header a Markdown report.

## Candidate thresholds

Minimálne sa ladia:

- `small_count_cutoff`;
- `stable_insertion_cutoff`;
- `near_sorted_insertion_max_count`;
- `near_sorted_disorder_max`;
- tri Shell max count limity;
- tri opportunistic merge min count limity, kde hodnota 0 znamená vypnuté;
- `cycle_min_count` a `cycle_equal_score_max`, iba ak je cycle zapnutý;
- `sample_pairs` z množiny `{4, 8, 12, 16}`.

## Generated artifacts

```text
generated/lox_profile_<target>.h
reports/lox_profile_<target>.md
```

Header obsahuje:

- `static const lox_profile_t`;
- profile format version;
- comments alebo optional macros s target identitou;
- exact benchmark input hash;
- generator version.

Report obsahuje:

- benchmark environment;
- coverage scenárov;
- zvolený profil;
- regret percentily;
- scenáre s najvyšším regretom;
- zoznam neoverených target vlastností.

## Quality gates pre profil

Generic profil nemusí vyhrať benchmarky, ale musí byť korektný.

Platformový profil môže byť označený `validated`, iba ak:

- žiadne correctness failure;
- všetky required scenarios sú prítomné;
- p95 regret voči best eligible neprekročí 1.20;
- maximum regret neprekročí 2.00;
- profil prejde integračnými testami;
- benchmark environment je jednoznačne identifikovaný.

Ak gate neprejde, profil sa môže uložiť ako experimentálny, ale nesmie sa nastaviť ako default target profil.
