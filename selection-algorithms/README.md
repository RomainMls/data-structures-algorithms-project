# Selection Algorithms

Ce projet SDA compare plusieurs façons de trouver le k-ième plus petit élément dans un tableau. Il contient une version par sélection simple, une version avec tas, QuickSelect et l'algorithme de Floyd-Rivest.

Le programme lance d'abord des tests de correction, puis mesure le temps CPU ainsi que le nombre de comparaisons et d'échanges sur différents types de tableaux.

## Contenu

- `main.c` : programme de test et de benchmark.
- `SelectionSelect.c`, `HeapSelect.c`, `QuickSelect.c`, `FRSelect.c` : implémentations des algorithmes.
- `IntArray.c/.h` : génération et manipulation des tableaux.
- `brief.pdf` : énoncé du projet.
- `report.pdf` : rapport du projet.

## Compilation et exécution

```bash
make
make run
```

On peut aussi lancer un algorithme précis :

```bash
./quickselect 10000 5000
./frselect 10000 5000
```

Pour nettoyer les fichiers générés :

```bash
make clean
```
