/* ========================================================================= *
 * SelectionSelect
 * Implementation of the SelectionSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include <stdio.h>
 #include "Select.h"

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    size_t min;
    for(size_t i = 0; i < length; i++){
        min = i;

        for(size_t j = i + 1; j < length; j++)
            if(compare(array, j, min) < 0)
                min = j; 
        
        swap(array, i, min);
        if(i == k)
            return k;
    }
    printf("Min not found in the array\n");
    return 0;
}
