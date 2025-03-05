/* ========================================================================= *
 * HeapSelect
 * Implementation of the HeapSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include <stdio.h>

static void min_heapify(void *array, size_t i, size_t heap_size, int (*compare)(const void *, size_t i, size_t j), void (*swap)(void *array, size_t i, size_t j)){
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    size_t smallest;
    if(l < heap_size && compare(array, l, i) < 0)
        smallest = l;
    else
        smallest = i;
    if(r < heap_size && compare(array, r, smallest) < 0)
        smallest = r;

    if(smallest != i){
        swap(array, i, smallest);
        min_heapify(array, smallest, heap_size, compare, swap);
    }
}

static void build_min_heap(void *array, size_t length, int (*compare)(const void *, size_t i, size_t j), void (*swap)(void *array, size_t i, size_t j)){
    for(int i = length / 2 - 1; i >= 0; i--){
        min_heapify(array, i, length, compare, swap);
    }
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{

    size_t heap_size = length;
    build_min_heap(array, length, compare, swap);

    for(int i = length - 1; i > 0; i--){
        if(k == length - heap_size)
            return 0;

        swap(array, i, 0);
        heap_size -= 1;
        min_heapify(array, 0, heap_size, compare, swap);
    }

    return 0;
}
