#ifndef _INTARRAY_H_
#define _INTARRAY_H_

#include <stddef.h>

/* ------------------------------------------------------------------------- *
 * Fills an array with increasing integers. If array is non NULL, it will
 * will be reinitialized with increasing values. Otherwise a new array will 
 * be allocated.
 * 
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
int *fillIncreasingArray(int* array, size_t length);


/* ------------------------------------------------------------------------- *
 * Fills an array with decreasing integers. If array is non NULL, it will
 * will be reinitialized with decreasing values. Otherwise a new array will 
 * be allocated.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
int *fillDecreasingArray(int* array, size_t length);


/* ------------------------------------------------------------------------- *
 * Fills an array with random integers.  If array is non NULL, it will
 * will be reinitialized with random values. Otherwise a new array will 
 * be allocated.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
int *fillRandomArray(int* array, size_t length);

/* ------------------------------------------------------------------------- *
 * Fills an array with a constant (random) value.  If array is non NULL, it will
 * will be reinitialized with a constant value. Otherwise a new array will 
 * be allocated.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
int *fillConstantArray(int *array, size_t length);

/* ------------------------------------------------------------------------- *
 * Swaps integers at position i and j in the array.
 *
 * PARAMETERS
 * array        The array
 * i,j          The positions to swap
 * ------------------------------------------------------------------------- */
void swapInt(void *array, size_t i, size_t j);

/* ------------------------------------------------------------------------- *
 * Compares the integers at positions i and j in the array. Return 0 if both
 * integers are equal, a positive number if array[i] is larger than array[j],
 * a negative number otherwise.
 * 
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
int compareInt(const void *array, size_t i, size_t j);

/* ------------------------------------------------------------------------- *
 * Copies the content of the array into the newarray.  If newarray is NULL,
 * a new array will be allocated and returned by the function.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
int *copyArray(int *array, size_t length, int *newarray);

/* ------------------------------------------------------------------------- *
 * Returns the number of times the function swapInt was called since the
 * last call to resetNbSwaps.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
size_t getNbSwaps(void);

/* ------------------------------------------------------------------------- *
 * Returns the number of times the function compareInt was called since the
 * last call to resetNbComparisons.
 *
 * ------------------------------------------------------------------------- */
size_t getNbComparisons(void);

/* ------------------------------------------------------------------------- *
 * Resets the counter of calls to swapInt.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
void resetNbSwaps(void);

/* ------------------------------------------------------------------------- *
 * Resets the counter of calls to compareInt.
 *
 * PARAMETERS
 * array        The array to fill
 * length       Number of elements in the array (0 < length)
 * ------------------------------------------------------------------------- */
void resetNbComparisons(void);

#endif // !_ARRAY_H_
