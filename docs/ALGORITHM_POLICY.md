# LoxSort — Algorithms and Selection Policy

## 1. Insertion sort

**Vlastnosti**

- stabilný;
- in-place;
- bez rekurzie;
- O(n²) worst case;
- veľmi nízky call a code-size overhead;
- výborný pre malé a skoro zoradené vstupy.

**Povinné implementačné pravidlo**

Posun pokračuje iba pri `compare(prev, key) > 0`. Pri rovnosti sa nesmie meniť pôvodné poradie.

**Použitie**

- všetky vstupy do `small_count_cutoff` bez analýzy;
- profilom ohraničené skoro zoradené vstupy;
- stabilný fallback bez scratch;
- posledný bezpečný fallback, ak iný algoritmus nie je oprávnený.

## 2. Shell sort

**Vlastnosti**

- nestabilný;
- in-place;
- bez rekurzie;
- nízky stack;
- praktický pre stredné polia.

Použije sa Ciura gap sequence rozšírená deterministickým pravidlom. Presná sekvencia musí byť jednotná v implementácii aj testoch.

**Použitie**

- všeobecné vstupy do profilového limitu podľa veľkosti elementu;
- fallback pri `NO_RECURSION`;
- fallback, ak introsort nie je skompilovaný.

## 3. Introsort

**Vlastnosti**

- nestabilný;
- in-place;
- priemerné O(n log n);
- worst-case O(n log n) vďaka heapsort fallbacku;
- bounded recursion;
- malé partition dokončuje insertion sort.

**Povinné ochrany**

- depth limit odvodený z `2 * floor(log2(n))`;
- 3-way partition alebo ekvivalentná ochrana proti duplicate-heavy vstupom;
- pivot selection bez out-of-range prístupov;
- tail-recursion elimination aspoň na jednej vetve;
- žiadna VLA.

**Použitie**

Predvolená všeobecná cesta pre väčšie vstupy, ak rekurzia nie je zakázaná.

## 4. Bottom-up merge sort

**Vlastnosti**

- stabilný;
- bez rekurzie;
- O(n log n);
- vyžaduje plný scratch buffer;
- predvídateľný access pattern.

**Scratch**

```text
required = count * element_size
```

Overflow sa deteguje pred násobením. Scratch sa nesmie prekrývať s input array.

**Použitie**

- `REQUIRE_STABLE`;
- count je nad insertion fallback rozsahom;
- merge je skompilovaný;
- scratch je dostatočný;
- alebo ako bežná výkonová cesta, keď target profil nastaví nenulový `merge_min_count` pre danú triedu elementu a scratch je dostatočný.

Generic profil môže mať všetky opportunistic merge prahy nastavené na 0, čím túto cestu vypne bez ovplyvnenia stabilného triedenia.

## 5. Cycle sort — optional

**Vlastnosti**

- nestabilný;
- in-place;
- minimalizuje počet zápisov;
- O(n²) comparisons;
- komplikované správanie pri duplicitách.

**Default**

`LOXSORT_ENABLE_CYCLE=OFF`.

**Oprávnenie**

Cycle sort sa môže použiť iba keď:

- používateľ nastavil `PREFER_MIN_WRITES`;
- stabilita nie je požadovaná;
- cycle je skompilovaný a profilom povolený;
- count je aspoň `cycle_min_count`;
- sampled `equal_score` neprekračuje `cycle_equal_score_max`.

Preferencia minimálnych zápisov nesmie znamenať tvrdenie, že výsledok má matematicky minimálny počet fyzických zápisov pre každý comparator a duplicate layout. Dokumentácia smie tvrdiť iba, že bola zvolená špecializovaná low-write stratégia.

## Element size classes

```text
SMALL  : element_size <= 4
MEDIUM : element_size <= 16
LARGE  : element_size > 16
```

Profil má osobitný Shell limit pre každú triedu.

## Nearly sorted classification

Pole je runtime označené ako skoro zoradené iba ak:

```text
disorder_score <= profile.near_sorted_disorder_max
```

Insertion sa napriek tomu použije iba do:

```text
count <= profile.near_sorted_insertion_max_count
```

Tento horný limit chráni pred katastrofálnym O(n²) výberom, ak malá vzorka minie neusporiadanú oblasť veľkého poľa.

## Stability table

| Algoritmus | Stabilný | Extra RAM | Rekurzia | Hlavný účel |
|---|---:|---:|---:|---|
| Insertion | áno | O(1) | nie | malé, skoro zoradené, fallback |
| Shell | nie | O(1) | nie | stredné, no-recursion |
| Introsort | nie | O(log n) stack | áno | všeobecné veľké vstupy |
| Merge | áno | n × element_size | nie | stabilita so scratch |
| Cycle | nie | O(1) | nie | preferencia nízkych zápisov |

## Zakázané rozhodnutia

Dispatcher nesmie:

- vybrať nestabilný algoritmus pri `REQUIRE_STABLE`;
- vybrať introsort pri `NO_RECURSION`;
- vybrať merge bez dostatočného scratch;
- vybrať cycle bez explicitnej preferencie;
- vykonať sampling pri malom vstupe;
- použiť profilový prah na obídenie hard constraintu.
