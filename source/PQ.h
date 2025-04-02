
/* Interface for a priority queue. The keys are of type 'void *' and will 
 * be compared using the 'compare' function given as argument to the function
 * pqCreate.
 */

#ifndef  PQ_H_DEFINED
#define  PQ_H_DEFINED

#include <stddef.h>
#include <stdbool.h>


/* Opaque structure of a bounded priority queue */
typedef struct PQ_t PQ;

/* ------------------------------------------------------------------------- *
 * Creates a priority queue.
 *
 * PARAMETERS
 * capacity         Maximum number of elements that can be stored in the queue.
 * compare          A function to compare two keys
 *
 * NOTE
 * The returned structure should be cleaned with `pqFree` after
 * usage.
 *
 * RETURN
 * pq              The priority queue.
 * ------------------------------------------------------------------------- */
PQ *pqCreate(size_t capacity, int (*compare)(const void *, const void *));

/* ------------------------------------------------------------------------- *
 * Frees the memory allocated for the priority queue.
 *
 * PARAMETERS
 * pq              The priority queue to free
 * ------------------------------------------------------------------------- */
void pqFree(PQ* pq);

/* ------------------------------------------------------------------------- *
 * Inserts an object in the priority queue. If the queue is full,
 * the element is not added.
 *
 *
 * PARAMETERS
 * pq              The priority queue
 * key             The key to be stored in the queue
 *
 * RETURN
 * opened           True if the object was successfully added, false if the
 *   			    queue is full
 * ------------------------------------------------------------------------- */
bool pqInsert(PQ* pq, void *key);

/* ------------------------------------------------------------------------- *
 * Returns the maximum key currently stored in the priority queue. The queue
 * must contain at least one element (otherwise calling the function results in
 * an undefined behavior).
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * max              The maximum key
 * ------------------------------------------------------------------------- */
void *pqGetMax(const PQ* pq);

/* ------------------------------------------------------------------------- *
 * Returns the maximum key currently stored in the priority queue and extracts
 * it from the queue. The queue must contain at least one key (otherwise 
 * calling the function results in an undefined behavior).
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * max              The maximum key
 * ------------------------------------------------------------------------- */
void *pqExtractMax(PQ* pq);

/* ------------------------------------------------------------------------- *
 * Return the size of the queue (i.e. the number of keys currently stored
 * in the queue).
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * size 		    The size of the queue
 * ------------------------------------------------------------------------- */
size_t pqSize(const PQ* pq);

/* ------------------------------------------------------------------------- *
 * Return the capacity of the queue.
 *
 * PARAMETERS
 * pq              The priority queue
 *
 * RETURN
 * capacity 	   The capacity of the queue
 * ------------------------------------------------------------------------- */
size_t pqCapacity(const PQ* pq);


#endif // PQ_H_DEFINED
