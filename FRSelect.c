/* ========================================================================= *
 * FRSelect
 * Implementation of the Floyd-Rivest algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include <math.h>
 #include <stdlib.h>

static void swapSample(size_t *sample, size_t i, size_t j)
{
    size_t tmp = sample[i];
    sample[i] = sample[j];
    sample[j] = tmp;
}

static size_t sample_median(size_t *sample, size_t a, size_t b, size_t c, void *array,
                            int (*compare)(const void *, size_t i, size_t j))
{
    if (compare(array, sample[a], sample[b]) > 0) {
        size_t temp = a;
        a = b;
        b = temp;
    }

    if (compare(array, sample[c], sample[a]) < 0)
        return a;

    if (compare(array, sample[c], sample[b]) > 0)
        return b;

    return c;
}

// partitions the array into three categories: 'less', 'equal' and 'greater'
static size_t sample_partition3(size_t * sample, size_t p, size_t r, void *array,
                                int (*compare)(const void *, size_t i, size_t j))
{
    size_t pivot = sample_median(sample, p, (p+r)/2, r - 1, array, compare);
    swapSample(sample, pivot, r);

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
        if(compare(array, sample[j], sample[pivot]) < 0)
        {
            // put element in 'less' category
            swapSample(sample, i, j);

            swapSample(sample, k, i);

            k++;
            i++;
            j++;
        }
        else
        {
            if(compare(array, sample[j], sample[pivot]) == 0)
            {
                // put element in 'equal' category
                swapSample(sample, i, j);

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
        swapSample(sample, pivot, middleEqualCategory);
        if(i != r)
            swapSample(sample, i, r);

        return middleEqualCategory;
    }
    else
    {
        if(i != pivot)
            swapSample(sample, i, pivot);

        return i;
    }
}

static size_t sample_quickSelect(size_t *sample, size_t p, size_t r, size_t k, void *array,
                                 int (*compare)(const void *, size_t i, size_t j))
{
    if(p > r)
        return -1;

    if(p == r && p == k)
        return p;

    size_t q = sample_partition3(sample, p, r, array, compare);

    if(q == k)
        return q;

    if(q > k)
        return sample_quickSelect(sample, p, q-1, k, array, compare);

    else
        return sample_quickSelect(sample, q+1, r, k, array, compare);
}

static size_t * create_sample(size_t p, size_t r, size_t sampleSize)
{
    size_t * sample = malloc(sampleSize * sizeof(size_t));

    for(size_t i = 0; i < sampleSize; i++)
    {
        size_t randomIndex = p + rand() % (r + 1 - p);
        sample[i] = randomIndex;
    }

    return sample;
}

static size_t pivot_from_sample(void *array, size_t p, size_t r, size_t k,
                                int (*compare)(const void *, size_t i, size_t j))
{
    size_t sampleSize = sqrt(r + 1 - p);
    size_t correspondingK = k * sampleSize / (r + 1 - p);
    if(correspondingK >= sampleSize)
        correspondingK = sampleSize - 1;

    size_t * sample = create_sample(p, r, sampleSize);

    size_t sample_pivot = sample_quickSelect(sample, 0, sampleSize - 1, correspondingK, array, compare);
    size_t pivotIndex = sample[sample_pivot];

    free(sample);

    return pivotIndex;
}

// partitions the array into three categories: 'less', 'equal' and 'greater'
// with a pivot chosen using a sample of the array.
static size_t partition3_with_sample_pivot(void *array, size_t p, size_t r, size_t k,
                                          int (*compare)(const void *, size_t i, size_t j),
                                          void (*swap)(void *array, size_t i, size_t j))
{
    size_t pivot = pivot_from_sample(array, p, r, k, compare);

    if(pivot != r)
        swap(array, pivot, r);

    pivot = r;

    /*
     * INVARIANT:
     * i indicates the first element of the 'greater' category.
     * j indicates the element we are analysing.
     * l indicates the first element of the 'equal' category.
     * j - 1 indicates the last element of the 'greater' category.
     */
    size_t i = p, j = p, l = p;
    while(j < pivot)
    {
        if(compare(array, j, pivot) < 0)
        {
            // put element in 'less' category
            if(i != j)
                swap(array, i, j);

            if(l != i)
                swap(array, l, i);

            l++;
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
    if (i > l)
    {
        size_t middleEqualCategory = (i - 1 - l)/2 + l;
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

static size_t FRSelect(void *array, size_t p, size_t r, size_t k,
                       int (*compare)(const void *, size_t i, size_t j),
                       void (*swap)(void *array, size_t i, size_t j))
{
    if(p > r)
        return 0;

    if(p == r && p == k)
        return p;

    size_t q = partition3_with_sample_pivot(array, p, r, k, compare, swap);

    if(q == k)
        return q;

    if(q > k)
        return FRSelect(array, p, q - 1, k, compare, swap);

    else
        return FRSelect(array, q+1, r, k, compare, swap);
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    return FRSelect(array, 0, length - 1, k, compare, swap);
}
