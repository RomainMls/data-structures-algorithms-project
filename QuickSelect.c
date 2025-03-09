/* ========================================================================= *
 * QuickSelect
 * Implementation of the QuickSelect algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include "IntArray.h"
 #include <stdio.h>

static size_t median(void *array, size_t a, size_t b, size_t c,
                     int (*compare)(const void *, size_t i, size_t j))
{
    if(compare(array, a, b) > 0)
    {
        if(compare(array, c, a) > 0)
            return a;

        else
        {
            if(compare(array, b, c) > 0)
                return b;

            return c;
        }
    }
    else
    {
        if(compare(array, a, c) > 0)
            return a;

        else
        {
            if(compare(array, b, c) < 0)
                return b;

            return c;
        }
    }
}

// partitions the array into two gategories: 'less or equal' and 'greater'
static size_t partition(void *array, size_t p, size_t r,
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
     * j - 1 indicates the last element of the 'greater' category.
     */
    size_t i = p, j = p;
    while(j < pivot)
    {
        if(compare(array, pivot, j) >= 0)
        {
            // include element j in the 'less or equal' category
            if(i != j)
                swap(array, i, j);

            i++;
        }
        // element j is already in the 'greater' category
        j++;
    }
    if(i != pivot)
        swap(array, i, pivot);

    return i;
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
                // put element in 'equl' category
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
    size_t middleEqualCategory = (i - 1 - k)/2 + k;
    if (i > k)
    {
        swap(array, pivot, middleEqualCategory);
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

static size_t select_r(void *array, size_t p, size_t r, size_t k,
                       int (*compare)(const void *, size_t i, size_t j),
                       void (*swap)(void *array, size_t i, size_t j))
{
    if(p > r)
    {
        printf("ERROR select_r(): given sub array bounds unvalid\n");
        return -1;
    }

    if(p == r && p == k)
        return p;

    size_t q = partition3(array, p, r, compare, swap);

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
