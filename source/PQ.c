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
        return NULL;

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

    while(i > 0 && pq->array[i/2] < pq->array[i])       // i/2 is the parent of i
    {
        void *temp = pq->array[i/2];
        pq->array[i/2] = pq->array[i];
        pq->array[i] = temp;

        i = i/2;
    }

    return true;
}

void *pqGetMax(const PQ* pq)
{
    return pq->array[0];
}

void *pqExtractMax(PQ* pq)
{
    if(pq->size < 1)
        return NULL;

    void *max = pq->array[0];
    pq->array[0] = pq->array[pq->size-1];
    pq->size--;
    // maxheapify(pq->array, 0);
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