#include "PQ.h"

#include <stdlib.h>

struct PQ_t
{
    void **array;
    size_t capacity;
    size_t size;
    int (*compare)(const void *, const void *);
};

PQ *pqCreate(size_t capacity, int (*compare)(const void *, const void *))
{
    PQ *newPQ = malloc(sizeof(PQ));
    if(newPQ == NULL)
        return NULL;

    newPQ->array = malloc(capacity * sizeof(void *));
    if(newPQ->array == NULL)
    {
        free(newPQ);
        return NULL;
    }

    newPQ->capacity = capacity;
    newPQ->size = 0;
    newPQ->compare = compare;

    return newPQ;
}

void pqFree(PQ* pq)
{
    free(pq->array);
}

bool pqInsert(PQ* pq, void *key)
{
    if(pq->size >= pq->capacity)
        return false;

    pq->size++;
    size_t i = pq->size - 1;
    pq->array[i] = key;

    // max heapify
    while(i > 0 && pq->compare(pq->array[i/2], pq->array[i]) < 0)       // i/2 is the parent of i
    {
        void *temp = pq->array[i/2];
        pq->array[i/2] = pq->array[i];
        pq->array[i] = temp;

        i /= 2;
    }

    return true;
}

void *pqGetMax(const PQ* pq)
{
    if(pq->size > 0)
        return pq->array[0];

    return NULL;
}

static void max_heapify(PQ *pq, size_t i)
{
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    size_t max;
    if(l < pq->size && pq->compare(pq->array[l], pq->array[i]) > 0)
        max = l;
    else
        max = i;
    if(r < pq->size && pq->compare(pq->array[r], pq->array[max]) > 0)
        max = r;

    if(max != i)
    {
        // swap i et max
        void *tmp = pq->array[i];
        pq->array[i] = pq->array[max];
        pq->array[max] = tmp;
        max_heapify(pq, max);
    }
}

void *pqExtractMax(PQ* pq)
{
    if(pq->size < 1)
        return NULL;

    void *max = pq->array[0];
    pq->array[0] = pq->array[pq->size-1];
    pq->size--;
    max_heapify(pq, 0);
    return max;
}

size_t pqSize(const PQ* pq)
{
    return pq->size;
}

size_t pqCapacity(const PQ* pq)
{
    return pq->capacity;
}