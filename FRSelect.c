/* ========================================================================= *
 * FRSelect
 * Implementation of the Floyd-Rivest algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include <math.h>

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
    while(right > left){
        if(right - left > 600){
            size_t n = right - left + 1;
            size_t i = k - left + 1;
            double z = log(n);
            double s = 0.5 * exp(2 * z/3);
            double sd = 0.5 * sqrt(z * s * (n-s)/n) * sign(i - n/2);
            size_t newLeft = max(left, k - i * (s/n) + sd);
            size_t newRight = min(right, k + (n - i) * (s/n) + sd);
            select_r(array, newLeft, newRight, k, compare, swap);
        }
        size_t i = left;
        size_t j = right;
        swap(array, left, k);
        if(compare(array, right, k) < 0)
            swap(array, right, left);

        while(i < j){
            swap(array, i, j);
            i++;
            j--;
            while(compare(array, i, k) < 0)
                i++;
            while(compare(array, j, k) > 0)
                j++;
        }
        if(compare(array, left, k) == 0)
            swap(array, left, j);

        else{
            j++;
            swap(array, j, right);
        }
        if(j <= k)
            left = j + 1;

        if(k <= j)
            right = j - 1;
    }
    return 0;
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    return select_r(array, 0, length - 1, k, compare, swap);
}
