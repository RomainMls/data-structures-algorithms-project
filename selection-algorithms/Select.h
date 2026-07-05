#ifndef _SELECT_H_
#define _SELECT_H_

#include <stddef.h>

/* ------------------------------------------------------------------------- *
 * Returns the position of the k-th smallest element in an array of integers.
 * The function may permute the element in the array.
 *
 * PARAMETERS
 * array        The array to select from
 * length       Number of elements in the array (0 < length)
 * k            The rank of the element to select (0 <= k < length)
 * ------------------------------------------------------------------------- */
size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j));

#endif // !_SELECT_H_
