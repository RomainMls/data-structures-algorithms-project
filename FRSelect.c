/* ========================================================================= *
 * FRSelect
 * Implementation of the Floyd-Rivest algorithm
 * ========================================================================= */

 #include <stddef.h>
 #include "Select.h"
 #include <math.h>
 #include <stdlib.h>

static int sign(double x)
{
	if (x < 0)
		return -1;

	if (x > 0)
		return 1;

	return 0;
}

static size_t min(size_t a, size_t b)
{
    if(a > b)
        return b;

    return a;
}

static size_t max(size_t a, size_t b)
{
    if(a > b)
        return a;

    return b;
}

static size_t FRSelect(void *array, size_t left, size_t right, size_t k,
                       int (*compare)(const void *, size_t i, size_t j),
                       void (*swap)(void *array, size_t i, size_t j))
{
	while (right > left) {
		if (right - left > 600) {
			// Choosing a small subarray
			// S based on sampling.
			// 600, 0.5 and 0.5
			// are arbitrary constants
			int n = right - left + 1;
			int i = k - left + 1;
			double z = log(n);
			double s = 0.5 * exp(2 * z / 3);
			double sd = 0.5 * sqrt(z * s
								* (n - s) / n)
						* sign(i - n / 2);

			int newLeft = max(left,
							(size_t)(k - i * s / n + sd));

			int newRight = min(right,
							(size_t)(k + (n - i) * s / n
									+ sd));

			FRSelect(array, newLeft, newRight, k, compare, swap);
		}

		long i = left;
		long j = right;
		swap(array, left, k);

        if(compare(array, right, k) > 0)
        {
            swap(array, left, right);
        }

		while (i < j) {
			swap(array, i, j);
			i++;
			j--;
            while(compare(array, i, k) < 0)
                i++;

            while(compare(array, j, k) > 0)
                j--;
		}

        if(compare(array, left, k) == 0)
            swap(array, left, j);

        else
        {
            j++;
            swap(array, right, j);
        }

		// Adjust the left and right pointers
		// to select the subarray having k
		if (j <= k)
			left = j + 1;
		if (k <= j)
			right = j - 1;
	}
	return k;
}

size_t select(void *array, size_t length, size_t k,
              int (*compare)(const void *, size_t i, size_t j),
              void (*swap)(void *array, size_t i, size_t j))
{
    return FRSelect(array, 0, length - 1, k, compare, swap);
}
