/* ========================================================================= *
 * QuickSelect
 * Implementation of the QuickSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include "IntArray.h"
 #include <stdio.h>

static size_t partition(void *array, size_t p, size_t r,
                 int (*compare)(const void *, size_t i, size_t j),
                 void (*swap)(void *array, size_t i, size_t j))
{
    swap(array, p + (r - p)/2, r);
    size_t i = p, j = p;

    while(j < r){

        if(compare(array, r, j) >= 0){
            swap(array, i, j);
            i++;
        }
        j++;
    }
    swap(array, i, r);
    return i;
}

static size_t select_r(void *array, size_t p, size_t r, size_t k,
    int (*compare)(const void *, size_t i, size_t j),
    void (*swap)(void *array, size_t i, size_t j))
{
    if(p > r){
        printf("ERROR select_r(): given sub array bounds unvalid\n");
        return 0;
    }

    if(p == r && p == k)
        return p;

    size_t q = partition(array, p, r, compare, swap);

    if(q == k)
        return q;

    if(q > k)
        return select_r(array, p, q - 1, k, compare, swap);

    else
        return select_r(array, q+1, r, k, compare, swap);
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    return select_r(array, 0, length - 1, k, compare, swap);
}
