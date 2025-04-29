
/*
 * Usage:
 * Pour tester l'algorithme sur un fichier csv:
 * ./bp <disksize> <fichier.csv>
 * Pour tester l'algorithme sur un problème généré aléatoirement:
 * ./bp <disksize> <number_of_files>
 */

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

// necessary for some string function
#define _POSIX_C_SOURCE 200809L

static List *generateRandomFiles(size_t diskSize, size_t nbFiles)
{
    List *files = llCreateEmpty();

    char buffer[1024];
    for (size_t i = 0; i < nbFiles; i++)
    {
        sprintf(buffer, "f%zu", i);

        //Strdup did not work, so I remplace it by malloc + strcpy
        char *name_copy = malloc(strlen(buffer) + 1);
        if(!name_copy){
            fprintf(stderr, "Memory allocation failed for file name\n");
            return NULL;
        }

        strcpy(name_copy, buffer);

        File *f = fileCreate(name_copy, rand() % diskSize + 1);
        llInsertLast(files, f);
    }
    return files;
}

int main(int argc, char *argv[])
{
    // srand(time(NULL));

    srand(42);

    size_t diskSize = 1000000;
    List *disks;

    if(argc != 3)
    {
        printf("Missing arguments. Usage: <nbFiles> <nbTests>\n");
        return 0;
    }
    size_t nbFiles = strtoull(argv[1], NULL, 10);
    int nbTests = strtoull(argv[2], NULL, 10);

    List *files;

    int i = 0;
    long totalLost = 0;
    double totalTime = 0;
    while(i < nbTests)
    {
        disks = llCreateEmpty();
        files = generateRandomFiles(diskSize, nbFiles);
        clock_t start = clock();

        binpacking(diskSize, files, disks);

        clock_t end = clock();

        totalTime += ((double)(end - start)) / (double)CLOCKS_PER_SEC;

        Node *current = llHead(disks);
        Disk *currentFile;
        while(current != NULL)
        {
            currentFile = llData(current);
            totalLost += diskFreeSpace(currentFile);
            current = llNext(current);
        }

        llFree(disks);
        llFree(files);
        i++;
    }
    double avgTime = totalTime / nbTests;
    long avgLost = totalLost / nbTests;

    printf("avg time: %f\navg lost = %ld\n", avgTime, avgLost);

    return 0;
}
