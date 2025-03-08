/* ========================================================================= *
 * FRSelect
 * Implementation of the Floyd-Rivest algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include <math.h>
 #include <stdlib.h>
 #include <stdio.h>

static double sign(double x){
    if(x >= 0)
        return 1;

    return -1;
}

static size_t max(size_t a, size_t b){
    if(a - b > 0)
        return a;

    return b;
}

static size_t min(size_t a, size_t b){
    if(a - b > 0)
        return b;

    return a;
}

static size_t select_r(void *array, size_t left, size_t right, size_t k,
                      int (*compare)(const void *, size_t i, size_t j),
                      void (*swap)(void *array, size_t i, size_t j))
{
    while (left < right) {
        // Use recursion for large arrays for better performance
        if (right - left > 600) {
            size_t n = right - left + 1;
            size_t i = k - left + 1;
            double z = log(n);
            double s = 0.5 * exp(2 * z / 3);

            // Standard deviation calculation
            double sd;
            if (i > n/2) {
                sd = 0.5 * sqrt(z * s * (n - s) / n);
            } else {
                sd = -0.5 * sqrt(z * s * (n - s) / n);
            }

            // Recursively sample a smaller interval
            size_t newLeft = (k - i * s / n + sd > left) ?
                             (size_t)(k - i * s / n + sd) : left;
            size_t newRight = (k + (n - i) * s / n + sd < right) ?
                              (size_t)(k + (n - i) * s / n + sd) : right;

            select_r(array, newLeft, newRight, k, compare, swap);
        }

        // Base case - perform partition
        // Move pivot to the left position
        swap(array, left, k);

        // Initial index for partitioning
        size_t i = left + 1;
        size_t j = right;

        // Ensure pivot >= rightmost element
        if (compare(array, right, left) < 0) {
            swap(array, left, right);
        }

        // Main partitioning loop
        while (i < j) {
            // Swap elements in wrong positions
            swap(array, i, j);
            i++;
            j--;

            // Find element >= pivot from left
            while (compare(array, i, left) < 0) {
                i++;
            }

            // Find element <= pivot from right
            while (compare(array, left, j) < 0) {
                j--;
            }
        }

        // Handle final pivot placement
        if (compare(array, left, j) == 0) {
            swap(array, left, j);
        } else {
            j++;
            swap(array, j, right);
        }

        // Adjust search boundaries based on pivot position
        if (j <= k) {
            left = j + 1;
        }
        if (k <= j) {
            right = j - 1;
        }
    }

    return k;
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    return select_r(array, 0, length - 1, k, compare, swap);
}
