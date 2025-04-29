
/*
 * Usage:
 * Pour tester l'algorithme sur un fichier csv:
 * ./bp <disksize> <fichier.csv>
 * Pour tester l'algorithme sur un problème généré aléatoirement:
 * ./bp <disksize> <number_of_files>
 */

// necessary for some string function
#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "Disk.h"
#include "File.h"
#include "BP.h"

static List *readFiles(char *filename)
{
    FILE *csvfile = fopen(filename, "r");
    if (!csvfile)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    List *files = llCreateEmpty();
    char line[1024];
    while (fgets(line, 1024, csvfile))
    {
        if (strlen(line) <= 1)
            continue;
        char *name = strdup(strtok(line, ","));
        size_t size = strtoull(strtok(NULL, ",\n"), NULL, 10);
        File *file = fileCreate(name, size);
        llInsertLast(files, file);
    }
    fclose(csvfile);
    return files;
}

static List *generateRandomFiles(size_t diskSize, size_t nbFiles)
{
    List *files = llCreateEmpty();

    char buffer[1024];
    for (size_t i = 0; i < nbFiles; i++)
    {
        sprintf(buffer, "f%zu", i);
        File *f = fileCreate(strdup(buffer), rand() % diskSize + 1);
        llInsertLast(files, f);
    }
    return files;
}

static void printSolution(size_t diskSize, List *files, List *disks)
{
    Node *p;

    size_t totalsize = 0;
    p = llHead(files);
    while (p)
    {
        totalsize += fileSize(llData(p));
        p = llNext(p);
    }
    printf("Total file sizes: %zu\n", totalsize);
    printf("Disk size: %zu\n", diskSize);
    printf("Minimum/Maximum number of disks: %zu/%zu\n",
           (size_t)ceill((double)totalsize / (double)diskSize),
           llLength(files));
    printf("Number of disks in solution: %zu\n", llLength(disks));
    printf("Lost space: %zu\n", llLength(disks) * diskSize - totalsize);

    if (llLength(files) > 100)
    {
        printf("Too many files. No print.\n");
        return;
    }

    p = llHead(disks);
    int indexdisk = 0;
    while (p)
    {
        Disk *d = llData(p);
        printf("Disk %d: used space: %zu, free space: %zu\n", indexdisk++, diskUsedSpace(d), diskFreeSpace(d));
        List *dfiles = diskFiles(d);
        printf("Files (%zu): ", llLength(dfiles));
        Node *pf = llHead(dfiles);
        while (pf)
        {
            File *f = llData(pf);
            printf("[%s, %zu]", fileName(f), fileSize(f));
            pf = llNext(pf);
        }
        printf("\n");
        p = llNext(p);
    }
}

int main(int argc, char *argv[])
{
    // srand(time(NULL));
    
    srand(42);

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <disk_size> [<csv_file>|number_of_files]\n", argv[0]);
        return 1;
    }

    size_t diskSize = strtoull(argv[1], NULL, 10);

    int i = 0;
    while (argv[2][i] != '\0' && isdigit(argv[2][i]))
        i++;

    List *files;
    if (argv[2][i] == '\0')
    {
        // random files
        size_t nbFiles = strtoull(argv[2], NULL, 10);
        files = generateRandomFiles(diskSize, nbFiles);
    }
    else
        files = readFiles(argv[2]);

    List *disks = llCreateEmpty();

    clock_t start = clock();

    binpacking(diskSize, files, disks);

    clock_t end = clock();

    printf("CPU time: %f\n", (double)(end - start) / (double)CLOCKS_PER_SEC);

    printSolution(diskSize, files, disks);

    llFreeData(disks, diskFree);
    llFreeData(files, fileFree);

    return 0;
}
