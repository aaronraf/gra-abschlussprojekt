# Cache Simulation und Analyse (A11)

## Beitrag jedes Gruppenmitglieds
- **Lie Aditya Bryan**:
- **Jovan Rio Tjandra**:
- **Aaron Rafael Thamin**:

## Theoretischer Teil

In modernen Prozessoren werden folgenden Größen und Latenzen für direkt- und vierfach-assoziativen Cache häufig verwendet. 

### Größen
- **Direkt-assoziativ**
    - **L1-Cache (Data & Instruction)**: 32 KB bis 64 KB
    - **L2-Cache**: 256 KB bis 512 KB
    - **L3-Cache**: 4 MB bis 8 MB
- **4-fach-assoziativ**
    - **L1-Cache (Data & Instruction)**: 32 KB bis 64 KB
    - **L2-Cache**: 512 KB bis 1 MB
    - **L3-Cache**: 8 MB bis 64 MB

### Latenzen
| Komponent                 | Latenzen in Zyklen| 1 GHz (ns)       | 2 GHz (ns)       | 3 GHz (ns)       | 4 GHz (ns)       |  5 GHz (ns)      |
|---------------------------|-------------------|------------------|------------------|------------------|------------------|------------------|
| **L1-Cache**              | 1 - 4             | 1 - 4            | 0,5 - 2          | 0,333 - 1,333    | 0.25 - 1         | 0.2 - 0.8        |
| **L2-Cache**              | 5 - 12            | 5 - 12           | 2,5 - 6          | 1,667 - 4        | 1,25 - 3         | 1 - 2,4          |
| **L3-Cache**              | 20 - 40           | 20 - 40          | 10 - 20          | 6,667 - 13,333   | 5 - 10           | 4 - 8            |
| **Hauptspeicher (RAM)**   | 50 - 150          | 50 - 150         | 25 - 75          | 16,667 - 50      | 12,5 - 37,5      | 10 - 30          |


### Speicherzugriffsverhalten

- **Raumliche Lokalität (Zugriff auf nahegelegene Speicherplätze)**: Beide Caches profitieren davon, aber der 4-fach assoziative Cache nutzt die raumliche Lokalität besser aus, indem er mehrere nahegelegene Blöcke im selben Satz speichert.
- **Zeitliche Lokalität (erneuter Zugriff auf dieselben Speicherplätze kurz nach dem ersten Zugriff)**: Beide Cache-Speicher profitieren in ähnlicher Weise davon, obwohl der 4-fach-assoziative Cache potenziell mehrere Blöcke, auf die kürzlich zugegriffen wurde, im selben Satz behalten kann.
- **Sequentieller Zugriff**: Beide Caches bewältigen sequentielle Zugriffsmuster gut, aber der 4-fach-assoziative Cache kann aufgrund seiner größeren Flexibilität und geringeren Konfliktwahrscheinlichkeit besser mit Bursts sequentieller Zugriffe umgehen.
- **Zufälliger Zugriff**: Direkt assoziative Caches können stärker unter zufälligen Zugriffsmustern leiden, wenn verschiedene Speicherblöcke auf dieselbe Cache-Zeile abgebildet werden, was zu häufigen Cache-Misses führt. Der 4-fach assoziative Cache kann dieses Problem entschärfen, indem er mehr Möglichkeiten für die Zwischenspeicherung verschiedener Blöcke im gleichen Satz bietet.

## Simulation eines speicherintensiven Algorithmus

Als Beispiel verwenden wir das Algorithmus für eine Matrixmultiplikation.

## Beispiel für ein Speicherzugriffsmuster

```csv
# W/R, Adresse, Data

```

## Quelle
- Hennessy, John L /  Patterson, David A (2019): Computer architecture : a quantitative approach, Cambridge, Ma 2019.
- Drepper, Ulrich (2007): What Every Programmer Should Know About Memory 2007, URL: https://people.freebsd.org/~lstewart/articles/cpumemory.pdf, 02.07.2024
- Norvig, Peter (2014): Teach Yourself Programming in Ten Years, URL: http://norvig.com/21-days.html#answers, 03.07.2024.
- (2016): Intel® 64 and IA-32 Architectures Software Developer’s Manual 2016, URL: https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-1-manual.pdf, 03.07.2024
