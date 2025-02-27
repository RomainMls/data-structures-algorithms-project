#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "IntArray.h"
#include "Select.h"

static int checkselect(int *array, size_t length, size_t k, size_t value)
{
    if (value >= length)
    {
        printf("   Something went wrong. The returned position (%zu) is not in the array.\n", value);
        return 0;
    }

    int key = array[value];
    size_t nblower = 0;
    size_t nbequal = 0;

    for (size_t i = 0; i < length; i++)
    {
        if (array[i] < key)
            nblower++;
        else if (array[i] == key)
            nbequal++;
    }

    if (k < nblower || k >= nblower + nbequal)
    {
        printf("   The returned index does not indicate the %zuth element in the array"
               " (between %zuth and %zuth).\n",
               k, nblower, nblower + nbequal - 1);
        return 0;
    }
    return 1;
}

static inline double selectCPUTime(int *array, size_t length, size_t k)
{
    clock_t start = clock();
    select(array, length, k, compareInt, swapInt);
    return ((double)(clock() - start)) / CLOCKS_PER_SEC;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    // Arguments
    size_t n = 10000, k = n / 2;
    const size_t n_checktest = 1000;

    if (argc > 1)
        sscanf(argv[1], "%zu", &n);
    if (argc > 2)
        sscanf(argv[2], "%zu", &k);

    // Tests

    printf("Testing the select function for all values of k on a random array of length 1000...\n");

    int *array = fillRandomArray(NULL, n_checktest);
    int *copyarray = copyArray(array, n_checktest, NULL);

    for (size_t i = 0; i < n_checktest; i++)
    {
        size_t pos = select(array, n_checktest, i, compareInt, swapInt);
        if (!checkselect(array, n_checktest, i, pos))
        {
            printf("   k = %zu did not return the expected results. Stopping the tests.\n", i);
            exit(-1);
        }
        copyArray(copyarray, n_checktest, array);
    }

    free(array);
    free(copyarray);

    printf("=> The select function seems correct.\n\nComputing CPU time and numbers of swaps and comparisons.\n\n");

    printf("Array         Time [s]   (#swaps,#comps)\n");
    printf("------------  --------   -----------------------\n");

    array = fillIncreasingArray(NULL, n);
    resetNbSwaps();
    resetNbComparisons();
    printf("increasing    %f\t  (%zu,%zu)\n", selectCPUTime(array, n, k),
           getNbSwaps(), getNbComparisons());

    fillDecreasingArray(array, n);
    resetNbSwaps();
    resetNbComparisons();
    printf("decreasing    %f\t  (%zu,%zu)\n", selectCPUTime(array, n, k),
           getNbSwaps(), getNbComparisons());

    fillRandomArray(array, n);
    resetNbSwaps();
    resetNbComparisons();
    printf("random        %f\t  (%zu,%zu)\n", selectCPUTime(array, n, k),
           getNbSwaps(), getNbComparisons());

    fillConstantArray(array, n);
    resetNbSwaps();
    resetNbComparisons();
    printf("constant      %f\t  (%zu,%zu)\n", selectCPUTime(array, n, k),
           getNbSwaps(), getNbComparisons());

    free(array);

    return 0;
}
