/* ========================================================================= *
 * SelectionSelect
 * Implementation of the SelectionSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include <stdio.h>
 #include "Select.h"

static inline size_t selectionSelectMin(
    void *array, size_t length, size_t k,
    int (*compare)(const void *, size_t i, size_t j),
    void (*swap)(void *array, size_t i, size_t j)
){
    size_t min;

    for(size_t i = 0; i < length - 1; i++){
        min = i;

        for(size_t j = i + 1; j < length; j++)
            if(compare(array, j, min) < 0)
                min = j;

        swap(array, i, min);
        if(i == k)
            return k;
    }
    return length - 1;
}

static inline size_t selectionSelectMax(
    void *array, size_t length, size_t k,
    int (*compare)(const void *, size_t i, size_t j),
    void (*swap)(void *array, size_t i, size_t j)
){
    size_t max;

    for(size_t i = length - 1; i > 0; i--){
        max = i;

        for(size_t j = 0; j < i; j++)
            if(compare(array, j, max) > 0)
                max = j;

        swap(array, i, max);
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
        return selectionSelectMax(array, length, k, compare, swap);

    return selectionSelectMin(array, length, k, compare, swap);
}
