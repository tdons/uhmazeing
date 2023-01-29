#ifndef MZ_PQ_H
#define MZ_PQ_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct mz_mpq_t;

/**
 * Create a monotone priority queue.
 *
 * @param size_t size of the pq
 */
struct mz_mpq_t *mz_mpq_create_monotone(size_t);

/**
 * Insert an element into the priority queue in O(1)
 *
 * @param uint32_t the priority of the value, must be >= priority of the last popped element 
 * @param uint64_t the value of the element
 */
void mz_mpq_insert(struct mz_mpq_t *, uint32_t, uint64_t);

/**
 * Remove an element with lowest priority from the queue. (amortized O(1))
 *
 * Note: after this call returns element e, inserting an element with a lower priority than that
 *       associated with e will result in an assertion failure
 */
uint64_t mz_mpq_pop(struct mz_mpq_t *);

/**
 * Is the queue empty?
 */
bool mz_mpq_empty(const struct mz_mpq_t *);

/**
 * Does the queue contain a particular element? O(n)
 *
 * @param uint64_t the element to search for
 */
bool mz_mpq_contains(const struct mz_mpq_t *, uint64_t);

/**
 * Clean up the priority queue.
 */
void mz_mpq_destroy(struct mz_mpq_t *);

#endif /* #ifndef MZ_PQ_H */
