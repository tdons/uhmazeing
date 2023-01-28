#include <stdbool.h>
#include <stdio.h>

#include "../src/union_find.c"

#define same(x, y) (mzn_uf_find(uf, x) == mzn_uf_find(uf, y))

bool union_test()
{
	struct mzn_uf_t *uf = mzn_uf_create(7);
	if (mzn_uf_num_sets(uf) != 7 || same(0, 1)) {
		return false;
	}
	mzn_uf_union(uf, 0, 1);
	mzn_uf_union(uf, 0, 2);
	if (mzn_uf_num_sets(uf) != 5 || !same(0, 1) || !same(0, 2) || !same(1, 2)) {
		return false;
	}
	mzn_uf_union(uf, 3, 4);
	mzn_uf_union(uf, 4, 5);
	mzn_uf_union(uf, 5, 6);
	if (mzn_uf_num_sets(uf) != 2 || !same(3, 4) || !same(3, 5) || !same(3, 6) || !same(4, 5) || !same(4, 6) || !same(5, 6)) {
		return false;
	}
	for (int i = 0; i <= 2; i++) {
		for (int j = 3; j <= 6; j++) {
			if (same(i, j)) {
				return false;
			}
		}
	}
	mzn_uf_union(uf, 2, 6);
	if (mzn_uf_num_sets(uf) != 1) {
		return false;
	}
	for (int i = 0; i <= 6; i++) {
                for (int j = 0; j <= 6; j++) {
                        if (!same(i, j)) {
                                return false;
                        }
                }
        }
	return true;
}

bool path_compression_test()
{
	struct mzn_uf_t *uf = mzn_uf_create(16);
	// Set each entry up to point to its predecessor
	for (int i = 1; i < 16; i++) {
		uf->parent[i] = i - 1;
	}
#if 0
	printf("before path compression: ");
	for (int i = 0; i < 16; i++) {
		printf("%d ", uf->parent[i]);	
	}
	printf("\n");
#endif
	// Path compression should happen as part of the find operation.
	if (mzn_uf_find(uf, 15) != 0) {
		return false;
	}
#if 0
	printf("after path compression: ");
	for (int i = 0; i < 16; i++) {
		printf("%d ", uf->parent[i]);	
	}
	printf("\n");
#endif
	// Every entry should now point to the representative element (0),
	// making subsequent lookups cheap.
	for (int i = 0; i < 16; i++) {
		if (uf->parent[i] != 0) {
			return false;
		}
	}
	return true;
}

int main()
{
	if (!union_test()) {
		return 1;
	}
	if (!path_compression_test()) {
		return 2;
	}
	return 0;
}
