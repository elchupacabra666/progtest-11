# Fibonacciho kód

- **Hodnocení**: 5.0 bodů (žádný možný bonus)
- **Termín odevzdání**: 27.03.2023 11:59:59

## Zadání

Úkolem je vytvořit C/C++ funkce, které dokáží překódovat soubor v kódování UTF8 do Fibonacciho kódu a zpět.

Znaky ukládáme jako čísla – indexy do tabulky kódů, dnes nejčastěji UNICODE. Znaků je v UNICODE mnoho – 1048576+65536. Pokud by byly ukládány přímo jako binární čísla, byly by potřeba na reprezentaci jednoho znaku vždy 4 bajty. Většina znaků ale má kódy nižší, např. hodnoty menší než 128 (65536). Toho využívá kódování UTF-8, které podle zapisované hodnoty ukládá znak kompaktněji do 1 až 4 bajtů.

Kódování UTF-8 pro běžné textové soubory snižuje celkový objem ukládaných dat, není ale optimální. Při návrhu kódování UTF-8 byl zvolen způsob, kdy kódy jednotlivých znaků vždy končí na hranici celého bajtu. To je praktické pro rychlé načítání, ale není to optimální z hlediska délky.

### Fibonacciho kód

Pro paměťově efektivní ukládání celých čísel byly vyvinuty jiné kódy, např. kód Fibonacciho, který je založen na Fibonacciho posloupnosti. Fibonacciho posloupnost je posloupnost celých čísel, kde každé další číslo posloupnosti vznikne jako součet dvou předešlých čísel. Posloupnost má podobu: 

1, 1, 2, 3, 5, 8, ...

Čísla ve Fibonacciho kódu se ukládají jako bitové posloupnosti, které nastaveným bitem udávají přítomnost příslušného prvku Fibonacciho posloupnosti. Například číslo 9 lze zapsat jako:

```plaintext
  1  1  2  3  5  8  ...
  9 = 1 + 8  →  1  0  0  0  0  1
  9 = 1 + 8  →  0  1  0  0  0  1
  9 = 1 + 3 + 5 → 1  0  0  1  1
```

Možných zápisů je mnoho. Ze všech variant se vyberou pouze takové zápisy, kde se žádné číslo z Fibonacciho posloupnosti neopakuje vícekrát (tedy zápis 1 + 1 + 2 + 5 nebude použit). Nepřipustíme zápisy, kde se vyskytují sousední členy Fibonacciho posloupnosti (např. zápis 1 + 3 + 5 nebude připuštěn). Dvojici sousedních prvků Fibonacciho posloupnosti lze vždy nahradit jejich součtem, tedy následujícím prvkem v posloupnosti.

S těmito omezeními je již Fibonacciho kód jednoznačný. Pro prvních několik celých čísel má podobu:

```plaintext
  1 = 1        →  1
  2 = 2        →  0  1
  3 = 3        →  0  0  1
  4 = 1 + 3    →  1  0  1
  5 = 5        →  0  0  0  1
  ...
```

### Značení konce čísla

Pro označení konce čísla přidáme do sekvence domluvenou značku – jeden bit s hodnotou 1. Fibonacciho kód tak vždy končí jedničkou a přidáním další jedničky dostaneme sekvenci dvou jednotkových bitů, což jednoznačně identifikuje konec kódu. Například:

- Číslo 11: `001011`
- Sekvence čísel 3 a 2: `0011011`

### Ošetření hodnoty 0

Fibonacciho kód v této podobě nedokáže uložit hodnotu 0. Ošetříme to posunutím kódu o jednu pozici:

```plaintext
  0 ~ 1  →  1  1
  1 ~ 2  →  0  1  1
  2 ~ 3  →  0  0  1  1
  ...
```

### Zarovnání na celé bajty

Zakódujeme-li například posloupnost čísel `0 5 10`, potřebujeme celkem 13 bitů. Do souboru ukládáme celé bajty, nevyužité bity nastavíme na nulu. Díky tomu neovlivní padding kódovaný text.

Ukázka zakódování:

```plaintext
Kódy znaků:        0   5      10 
Bitové sekvence:   11  10011  001011
Seskupení:         1110011001011 
Doplnění na celé bajty: 1110011001011000 
Bajty (LSB vlevo): 11100110   01011000
Bajty hexadecimálně: 67  1A
```

## Funkce k implementaci

Úkolem je realizovat dvě funkce s následujícím rozhraním:

```cpp
bool utf8_to_fibonacci(const char *sourceFile, const char *destFile);
bool fibonacci_to_utf8(const char *sourceFile, const char *destFile);
```

Obě funkce přijímají dvě jména souborů – zdrojový a cílový. Čtou zdrojový soubor a zapisují překódovaný výsledek do cílového souboru. Návratovou hodnotou je příznak úspěchu (true) nebo chyby (false). Pokud se během operace cokoliv nepodaří (otevřít soubor, číst zdroj, zapisovat cíl, nesprávný formát dat, apod.), funkce vrací hodnotu `false`.

## Poznámky

- Pečlivě ošetřujte souborové operace. 
- Fibonacciho kód vyžaduje, aby každý znak byl ukončen dvojicí po sobě jdoucích jedničkových bitů.
- Kód by měl být schopný pracovat s velkými soubory bez nutnosti načítat celý soubor do paměti.
