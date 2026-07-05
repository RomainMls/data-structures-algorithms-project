# Bin Packing Heuristics

Ce projet SDA étudie plusieurs heuristiques pour le problème de bin packing. L'idée est de placer des fichiers sur des disques de taille fixe en limitant l'espace perdu, puis de comparer les stratégies en temps d'exécution et en qualité de placement.

Les algorithmes implémentés sont : naive, next-fit, worst-fit, best-fit et first-fit. Le projet utilise aussi plusieurs structures de données, notamment listes chaînées, tas/priorité et arbres binaires de recherche selon l'heuristique.

## Contenu

- `source/` : code C, `Makefile`, script de benchmark et petit fichier de test.
- `BPInstances/` : instances CSV utilisées pour tester les algorithmes.
- `stats/` : scripts Python et graphiques de comparaison.
- `brief.pdf` : énoncé du projet.
- `report.pdf` : rapport du projet.

## Compilation et exécution

```bash
cd source
make
make run
```

Exemple avec un exécutable précis :

```bash
./bpbestfit 1000000 test_20.csv
./bpfirstfit 1000000 200
```

Le script `script.sh` lance une série de benchmarks sur plusieurs tailles :

```bash
chmod +x script.sh
./script.sh
```

Pour nettoyer les fichiers générés :

```bash
make clean
```
