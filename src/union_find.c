#include <assert.h>

#include "union_find.h"

struct mz_uf_t {
	uint32_t *parent;
	// It can be shown that every node has rank ⌊lg n⌋ or less.  Hence, for worst-case n of 2^32-1 we have lg2(n) ≈ 32.
	uint8_t *rank;
#if MZ_UF_TRACK_SET_SIZES == 1
	uint32_t *size;
#endif
	uint32_t num_sets;
	uint32_t num_sets_initial;
};

struct mz_uf_t *mz_uf_create(uint32_t num_sets)
{
	struct mz_uf_t *uf = malloc(sizeof(*uf));
	uf->num_sets = num_sets;
	uf->parent = malloc(sizeof(uint32_t) * num_sets);
	uf->rank = calloc(sizeof(uint8_t), num_sets);
	uf->num_sets_initial = num_sets;
	for (uint32_t i = 0; i < num_sets; i++) {
		uf->parent[i] = i;
	}
#if MZ_UF_TRACK_SET_SIZES == 1
	uf->size = malloc(sizeof(uint32_t) * num_sets);
	for (uint32_t i = 0; i < num_sets; i++) {
		uf->size[i] = 1;
	}
#endif
	return uf;
}

void mz_uf_union(struct mz_uf_t *uf, uint32_t x, uint32_t y)
{
	assert(((void)"x and y oob check", x < uf->num_sets_initial && y < uf->num_sets_initial));
	x = mz_uf_find(uf, x);
	y = mz_uf_find(uf, y);
	if (x == y) {
		return;
	}
	--uf->num_sets;
	if (uf->rank[x] > uf->rank[y]) {
		uf->parent[y] = x;
#if MZ_UF_TRACK_SET_SIZES == 1
		uf->size[x] += uf->size[y];
#endif
	} else {
		uf->parent[x] = y;
#if MZ_UF_TRACK_SET_SIZES == 1
		uf->size[y] += uf->size[x];
#endif
		if (uf->rank[x] == uf->rank[y]) {
			assert(((void)"rank should not exceed 32", uf->rank[y] <= 32));
			++uf->rank[y];
		}
	}
}

uint32_t mz_uf_find(struct mz_uf_t *uf, uint32_t e)
{
	assert(((void)"e oob check", e < uf->num_sets_initial));
	uint32_t root = e, parent;
	while (uf->parent[root] != root) {
		root = uf->parent[root];
	}
	// Path compression.
	while (uf->parent[e] != root) {
		parent = uf->parent[e];
		uf->parent[e] = root;
		e = parent; 
	}
	return root;
}

#if MZ_UF_TRACK_SET_SIZES == 1
uint32_t mz_uf_sizeof(struct mz_uf_t *uf, uint32_t e)
{
	return uf->size[mz_uf_find(uf, e)];
}
#endif

uint32_t mz_uf_num_sets(const struct mz_uf_t *uf)
{
	return uf->num_sets;
}

void mz_uf_destroy(struct mz_uf_t *uf)
{
	free(uf->parent);
	free(uf->rank);
#if MZ_UF_TRACK_SET_SIZES == 1
	free(uf->size);
#endif
	free(uf);
}
