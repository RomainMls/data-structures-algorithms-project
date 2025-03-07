/* ========================================================================= *
 * HeapSelect
 * Implementation of the HeapSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include <stdio.h>

static void min_heapify(void *array, size_t i, size_t heapSize,
                        int (*compare)(const void *, size_t i, size_t j),
                        void (*swap)(void *array, size_t i, size_t j))
{
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    size_t min;
    if(l < heapSize && compare(array, l, i) < 0)
        min = l;

    else
        min = i;

    if(r < heapSize && compare(array, r, min) < 0)
        min = r;

    if(min != i){
        swap(array, i, min);
        min_heapify(array, min, heapSize, compare, swap);
    }
}

static void build_min_heap(void *array, size_t length,
                           int (*compare)(const void *, size_t i, size_t j),
                           void (*swap)(void *array, size_t i, size_t j))
{
    for(size_t i = length / 2; i > 0; i--)
        min_heapify(array, i - 1, length, compare, swap);
}

static size_t heapselect_min(void *array, size_t length, size_t k,
                      int (*compare)(const void *, size_t i, size_t j),
                      void (*swap)(void *array, size_t i, size_t j))
{
    size_t heapSize = length;
    build_min_heap(array, length, compare, swap);

    for(size_t i = length - 1; i > 0; i--){

        // assume that the binary tree is heapified
        if(k == length - heapSize)
            return 0;

        // extract the minimum
        swap(array, i, 0);
        heapSize--;

        // heapifythe smaller binary tree
        min_heapify(array, 0, heapSize, compare, swap);
    }

    return 0;
}

static void max_heapify(void *array, size_t i, size_t heapSize,
                        int (*compare)(const void *, size_t i, size_t j),
                        void (*swap)(void *array, size_t i, size_t j))
{
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    size_t max;
    if(l < heapSize && compare(array, l, i) > 0)
        max = l;
    else
        max = i;
    if(r < heapSize && compare(array, r, max) > 0)
        max = r;

    if(max != i){
        swap(array, i, max);
        max_heapify(array, max, heapSize, compare, swap);
    }
}

static void build_max_heap(void *array, size_t length,
                           int (*compare)(const void *, size_t i, size_t j),
                           void (*swap)(void *array, size_t i, size_t j))
{
    for(size_t i = length / 2; i > 0; i--)
        max_heapify(array, i - 1, length, compare, swap);
}

static size_t heapselect_max(void *array, size_t length, size_t k,
                      int (*compare)(const void *, size_t i, size_t j),
                      void (*swap)(void *array, size_t i, size_t j))
{
    size_t heapSize = length;
    build_max_heap(array, length, compare, swap);

    for(size_t i = length - 1; i > 0; i--){

        // assume that the binary tree is heapified
        if(k == i)
            return 0;

        // extract the maximum
        swap(array, i, 0);
        heapSize --;

        // heapify the smaller binary tree
        max_heapify(array, 0, heapSize, compare, swap);
    }

    return 0;
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    if(k > length/2)
        // heapselect using a max_heap will find the solution sooner
        return heapselect_max(array, length, k, compare, swap);

    return heapselect_min(array, length, k, compare, swap);
}