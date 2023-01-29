#include <assert.h>
 
#include "pq.h"

struct bucket {
	struct bucket *next, *prev;
	uint64_t value;
};

static void eject(struct bucket *b)
{
	b->next->prev = b->prev;
	b->prev->next = b->next;
}

static void add_right(struct bucket *r, struct bucket *b)
{
	b->prev = r;
	b->next = r->next;
	r->next->prev = b;
	r->next = b;
}

struct mz_mpq_t {
	struct bucket *buckets;
	size_t sz;
	uint32_t prio;
};

struct mz_mpq_t *mz_mpq_create_monotone(size_t sz)
{
	struct mz_mpq_t *pq = malloc(sizeof(*pq));
	pq->sz = sz;
	pq->prio = 0;
	pq->buckets = malloc(sizeof(*pq->buckets) * sz);
	for (size_t i = 0; i < sz; i++) {
		pq->buckets[i].next = pq->buckets[i].prev = pq->buckets + i;
		pq->buckets[i].value = 0; /* unused */
	}
	return pq;
}

/** O(1) */
void mz_mpq_insert(struct mz_mpq_t *pq, uint32_t prio, uint64_t val)
{
	assert(((void)"insert would violate monotonicity", prio >= pq->prio));
	assert(((void)"prio too large", prio < pq->sz));
	struct bucket *b = malloc(sizeof(*b));
	b->value = val;
	add_right(pq->buckets + prio, b);
}

/** Amortized O(1) */
uint64_t mz_mpq_pop(struct mz_mpq_t *pq)
{
	uint64_t ret;
	while (pq->buckets[pq->prio].next == pq->buckets + pq->prio && pq->prio < pq->sz -1) {
		++pq->prio;
	}
	struct bucket *b = pq->buckets[pq->prio].next;
	ret = b->value;
	eject(b);
	free(b);
	return ret;
}

bool mz_mpq_empty(const struct mz_mpq_t *pq)
{
	uint32_t prio = pq->prio;
	while (pq->buckets[prio].next == pq->buckets + prio && prio < pq->sz -1) {
		++prio;
	}
	if (prio < pq->sz - 1) {
		return false;
	}
	return pq->buckets[prio].next == pq->buckets + prio;
}

/** O(n) */
bool mz_mpq_contains(const struct mz_mpq_t *pq, uint64_t needle)
{
	struct bucket *b;
	for (size_t i = pq->prio; i < pq->sz; i++) {
		b = pq->buckets[i].next;
		while (b != pq->buckets + i) {
			if (b->value == needle) {
				return true;
			}
			b = b->next;
		}
	}
	return false;
}


void mz_mpq_destroy(struct mz_mpq_t *pq)
{
	struct bucket *b;
	while (pq->prio < pq->sz) {
		b = pq->buckets[pq->prio].next;
		while (b != pq->buckets + pq->prio) {
			b = b->next;
			free(b->prev);
		}
		++pq->prio;
	}
	free(pq->buckets);
	free(pq);
}
