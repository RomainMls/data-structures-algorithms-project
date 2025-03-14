/* ========================================================================= *
 * QuickSelect
 * Implementation of the QuickSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include "IntArray.h"

static size_t median(void *array, size_t a, size_t b, size_t c,
                     int (*compare)(const void *, size_t i, size_t j))
{
    if (compare(array, a, b) > 0) {
        size_t temp = a;
        a = b;
        b = temp;
    }

    if (compare(array, c, a) < 0)
        return a;

    if (compare(array, c, b) > 0)
        return b;

    return c;
}

// partitions the array into three categories: 'less', 'equal' and 'greater'
static size_t partition3(void *array, size_t p, size_t r,
                         int (*compare)(const void *, size_t i, size_t j),
                         void (*swap)(void *array, size_t i, size_t j))
{
    size_t pivot = median(array, p, (p+r)/2, r, compare);

    if(pivot != r)
        swap(array, pivot, r);

    pivot = r;

    /*
     * INVARIANT:
     * i indicates the first element of the 'greater' category.
     * j indicates the element we are analysing.
     * k indicates the first element of the 'equal' category.
     * j - 1 indicates the last element of the 'greater' category.
     */
    size_t i = p, j = p, k = p;
    while(j < pivot)
    {
        if(compare(array, j, pivot) < 0)
        {
            // put element in 'less' category
            if(i != j)
                swap(array, i, j);

            if(k != i)
                swap(array, k, i);

            k++;
            i++;
            j++;
        }
        else
        {
            if(compare(array, j, pivot) == 0)
            {
                // put element in 'equal' category
                if(i != j)
                    swap(array, i, j);

                i++;
                j++;
            }
            else
            {
                // put element in 'greater' categoryy
                j++;
            }
        }
    }
    if (i > k)
    {
        size_t middleEqualCategory = (i - 1 - k)/2 + k;
        swap(array, pivot, middleEqualCategory);
        if(i != r)
            swap(array, i, r);

        return middleEqualCategory;
    }
    else
    {
        if(i != pivot)
            swap(array, i, pivot);

        return i;
    }
}

static size_t quick_select(void *array, size_t p, size_t r, size_t k,
                           int (*compare)(const void *, size_t i, size_t j),
                           void (*swap)(void *array, size_t i, size_t j))
{
    if(p > r)
        return 0;

    if(p == r && p == k)
        return p;

    size_t q = partition3(array, p, r, compare, swap);

    if(q == k)
        return q;

    if(q > k)
        return quick_select(array, p, q-1, k, compare, swap);

    else
        return quick_select(array, q+1, r, k, compare, swap);
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    return quick_select(array, 0, length - 1, k, compare, swap);
}
