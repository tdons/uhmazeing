#include <stdbool.h>
#include <stdio.h>

#include "../src/permutation.c"

uint8_t perms4[][4] = {
	{ 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 2, 1, 3 }, { 0, 2, 3, 1 }, { 0, 3, 1, 2 }, { 0, 3, 2, 1 },
	{ 1, 0, 2, 3 }, { 1, 0, 3, 2 }, { 1, 2, 0, 3 }, { 1, 2, 3, 0 }, { 1, 3, 0, 2 }, { 1, 3, 2, 0 },
	{ 2, 0, 1, 3 }, { 2, 0, 3, 1 }, { 2, 1, 0, 3 }, { 2, 1, 3, 0 }, { 2, 3, 0, 1 }, { 2, 3, 1, 0 },
	{ 3, 0, 1, 2 }, { 3, 0, 2, 1 }, { 3, 1, 0, 2 }, { 3, 1, 2, 0 }, { 3, 2, 0, 1 }, { 3, 2, 1, 0 },
};

bool test_4()
{
	struct mzn_perms_t *ps = mzn_perms_ofseq0to(4);
	for (uint32_t i = 0; i < ps->sz; i++) {
		for (uint8_t j = 0; j < ps->n; j++) {
			if (perms4[i][j] != ps->perms[i * ps->n + j]) {
				return false;
			}
		}
	}
	return true;
}

int main()
{
	if (!test_4()) {
		return 1;
	}
	
	return 0;
}
