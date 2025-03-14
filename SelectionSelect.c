/* ========================================================================= *
 * SelectionSelect
 * Implementation of the SelectionSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"

// sorts the array from left to right
// until element number k is found
static inline size_t selection_select_min(
    void *array, size_t length, size_t k,
    int (*compare)(const void *, size_t i, size_t j),
    void (*swap)(void *array, size_t i, size_t j)
){
    /*
     * INVARIANT:
     * The array is sorted from 0 to i - 1,
     * the array is unsorted from i to length -1.
     * array[0] is the minimum of array.
     */
    for(size_t i = 0; i < length - 1; i++)
    {
        // find the minimum of the nonsorted subarray
        size_t min = i;
        for(size_t j = i + 1; j < length; j++)
            // keep the most left minimum for swapping in order to preserve stability
            if(compare(array, j, min) < 0)
                min = j;

        // put the minimum on the sorted part of the array
        if(i != min)
            swap(array, min, i);    // stabiliy is preserved since min is the most left minimum

        if(i == k)
            return k;
    }
    return length - 1;
}

// sorts the array from right to left
// until element number k is found
static inline size_t selection_select_max(
    void *array, size_t length, size_t k,
    int (*compare)(const void *, size_t i, size_t j),
    void (*swap)(void *array, size_t i, size_t j)
){
    /*
     * INVARIANT:
     * The array is sorted from i + 1 to length,
     * the array is unsorted from 0 to i.
     * array[length - 1] is the maximum of array.
     */
    for(size_t i = length - 1; i > 0; i--)
    {
        // find the maximum of the nonsorted subarray
        size_t max = 0;
        for(size_t j = 1; j <= i; j++)
            if(compare(array, j, max) >= 0)
                // keep the most right maximum for swapping in order to preserve stability
                max = j;

        // put the maximum on the sorted part of the array
        if(i != max)
            swap(array, max, i);    // stability is preserved since max is the most right maximum

        if(i == k)
            return k;
    }
    return 0;
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    if(k > length/2)
        // selection_select_max will find the solution faster
        return selection_select_max(array, length, k, compare, swap);

    return selection_select_min(array, length, k, compare, swap);
}
