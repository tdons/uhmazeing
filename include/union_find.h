#ifndef MZN_UNION_FIND
#define MZN_UNION_FIND

#include <stdlib.h>

/**
 * A disjoint-set data structure maintains a collection S = { s_1, s_2, ..., s_n } of disjoint dynamic sets.
 * Each set is identified by a representative, which is some member of the set.
 * Introduction to Algorithms, Cormen et al.  chapter 21.1 p. 498
 *
 * Note:
 * This data structure is somewhat famous because the amortized running time of an operation is Θ(α(n)) where
 * α(n) is the inverse Ackermann function.  The inverse Ackermann function grows extraordinarily slowly,
 * so this factor is 4 or less for any n that can actually be written in the physical universe. This makes
 * disjoint-set operations practically amortized constant time.
 */

#define MZN_UF_TRACK_SET_SIZES 0

struct mzn_uf_t;

/**
 * Create a new disjoint set data structure
 *
 * @param uint32_t the number of disjoint sets to start with 
 */
struct mzn_uf_t *mzn_uf_create(uint32_t);

/**
 * Given two elements (not necessarily of different sets), merge the two sets.
 * 
 * After mzn_uf_union(uf, x, y) returns it is always the case that:
 * mzn_uf_find(uf, x) == mzn_uf_find(uf, y).
 *
 * @param uint32_t an element of a set
 * @param uint32_t an element of a set 
 */
void mzn_uf_union(struct mzn_uf_t *, uint32_t, uint32_t);

/**
 * @return the representative of the set to which e belongs.
 *
 * @param uint32_t an element of a set
 */
uint32_t mzn_uf_find(struct mzn_uf_t *, uint32_t);

#if MZN_UF_TRACK_SET_SIZES == 1
/**
 * @return the size of the set to which e belongs.
 */
uint32_t mzn_uf_sizeof(struct mzn_uf_t *, uint32_t);
#endif

/**
 * @return the number of disjoint sets in the forest.
 */
uint32_t mzn_uf_num_sets(const struct mzn_uf_t *);

/**
 * Destroy the union find datastructure.
 */
void mzn_uf_destroy(struct mzn_uf_t *);

#endif /* #ifndef MZN_UNION_FIND */
