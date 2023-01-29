#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "permutation.h"

static uint32_t fac(uint32_t x)
{
	uint32_t ret = 1;
	for (uint32_t i = 2; i <= x; i++) {
		ret *= i;
	}
	return ret;
}

static void generate_seq(struct mz_perms_t *ps)
{
	#define SWAP(arr, x, y) tmp = arr[x]; arr[x] = arr[y]; arr[y] = tmp;
	uint32_t ix = 0;
	uint8_t *set = calloc(sizeof(*set), ps->n);
	for (uint8_t i = 0; i < ps->n; i++) {
		set[i] = i;
	}
	memcpy(ps->perms + (ix++ * ps->n), set, ps->n);
	int32_t tmp, i, j, k, l = 0;
	while (true) {
		j = -1;
		for (i = ps->n - 2; i >= 0; i--) {
			if (set[i] < set[i + 1]) {
				j = i;
				break;
			}
		}
		if (j == -1) {
			break;
		}
		for (i = ps->n - 1; i >= j + 1; i--) {
			if (set[i] > set[j]) {
				l = i;
				break;
			}
		}
		SWAP(set, j, l);
		k = (ps->n - j - 1) / 2;
		for (i = 0; i < k; i++) {
			SWAP(set, j + i + 1, ps->n - i - 1);
		}
		memcpy(ps->perms + (ix++ * ps->n), set, ps->n);
	}
	assert(((void)"index check", ix == ps->sz));
	free(set);
	#undef SWAP
}

struct mz_perms_t *mz_perms_ofseq0to(uint8_t n)
{
	assert(((void)"n too large", n < 13));
	struct mz_perms_t *ps = malloc(sizeof(*ps));
	ps->n = n;
	ps->sz = fac(n);
	ps->perms = malloc(sizeof(*ps->perms) * ps->n * ps->sz);
	generate_seq(ps);
	return ps;
}

void mz_perms_destroy(struct mz_perms_t *ps)
{
	free(ps->perms);
}
