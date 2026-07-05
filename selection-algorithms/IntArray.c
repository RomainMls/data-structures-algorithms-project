#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "IntArray.h"

static const unsigned UPPER_BOUND = 1000003;
static size_t nbSwaps = 0;
static size_t nbComparisons = 0;

static int *allocateArray(size_t length)
{
    int *array = malloc(length * sizeof(int));
    if (array == NULL)
    {
        fprintf(stderr, "allocateArray: malloc error.\n");
        exit(-1);
    }
    return array;
}

int *copyArray(int *array, size_t length, int *newarray)
{
    if (newarray == NULL)
        newarray = allocateArray(length);
    for (size_t i = 0; i < length; i++)
        newarray[i] = array[i];
    return newarray;
}

int *fillIncreasingArray(int *array, size_t length)
{
    if (array == NULL)
        array = allocateArray(length);

    const unsigned STEP = 1 + UPPER_BOUND / length;
    array[0] = rand() % STEP;
    for (size_t i = 1; i < length; i++)
        array[i] = array[i - 1] + rand() % STEP;

    return array;
}

int *fillDecreasingArray(int *array, size_t length)
{
    if (array == NULL)
        array = allocateArray(length);

    const unsigned STEP = 1 + UPPER_BOUND / length;
    array[length - 1] = rand() % STEP;
    for (int i = length - 2; i >= 0; i--)
        array[i] = array[i + 1] + rand() % STEP;

    return array;
}

int *fillConstantArray(int *array, size_t length)
{
    if (array == NULL)
        array = allocateArray(length);

    int constant = rand() % UPPER_BOUND;

    for (size_t i = 0; i < length; ++i)
        array[i] = constant;

    return array;
}

int *fillRandomArray(int *array, size_t length)
{
    if (array == NULL)
        array = allocateArray(length);

    for (size_t i = 0; i < length; i++)
        array[i] = rand() % UPPER_BOUND;

    return array;
}

void swapInt(void *array, size_t i, size_t j)
{
    int temp = ((int *)array)[i];
    ((int *)array)[i] = ((int *)array)[j];
    ((int *)array)[j] = temp;
    nbSwaps++;
}

int compareInt(const void *array, size_t i, size_t j)
{
    nbComparisons++;
    return ((int *)array)[i] - ((int *)array)[j];
}

size_t getNbSwaps(void)
{
    return nbSwaps;
}

size_t getNbComparisons(void)
{
    return nbComparisons;
}

void resetNbSwaps(void)
{
    nbSwaps = 0;
}

void resetNbComparisons(void)
{
    nbComparisons = 0;
}
