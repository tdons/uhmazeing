#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../src/random.c"

bool test_seed()
{
	struct mz_rnd_t r;
	r.state[0] = r.state[1] = r.state[2] = r.state[3] = 0;
	mz_rnd_init(&r, 0);
	if (r.state[0] == 0 || r.state[1] == 0 || r.state[2] == 0 || r.state[3] == 0) {
		printf("expected state to be not-null after seeding but was %x %x %x %x\n", r.state[0], r.state[1], r.state[2], r.state[3]);
		return false;
	}
	return true;
}

bool test_bounded()
{
	struct mz_rnd_t r;
	mz_rnd_init(&r, 0);
	uint32_t arr[128];
	for (uint32_t n = 1; n < 128; n++) {
		uint32_t iters = 0;
		memset(&arr[0], 0, sizeof(arr));
		do {
			++iters;
			arr[mz_rnd_next(&r, n)] = 1;
			bool done = true;
			for (uint32_t i = 0; i < n; i++) {
				if (arr[i] == 0) {
					done = false;
					break;
				}
			}
			if (done) {
				break;
			}
		} while (true);
#if 0
		printf("%u done in %u iters\n", n, iters);
#endif
	}
	return true;
}

bool test_pow()
{
	struct mz_rnd_t r;
	mz_rnd_init(&r, 0);
	uint32_t arr[128];
	uint32_t n = 1;
	for (uint8_t e = 1; e < 7; e++) {
		n *= 2;
		uint32_t iters = 0;
		memset(&arr[0], 0, sizeof(arr));
		do {
			++iters;
			arr[mz_rnd_next2pow(&r, e)] = 1;
			bool done = true;
			for (uint32_t i = 0; i < n; i++) {
				if (arr[i] == 0) {
					done = false;
					break;
				}
			}
			if (done) {
				break;
			}
		} while (true);
#if 0
		printf("%u done in %u iters\n", n, iters);
#endif
	}
	return true;
}

bool test_double()
{
	struct mz_rnd_t r;
	mz_rnd_init(&r, 0);
	for (size_t i = 0; i < 100; i++) {
		double v = mz_rnd_nextdouble(&r);
		if (v < 0 || v >= 1) {
			return false;
		}
	}
	return true;
}

int main()
{
	if (!test_seed()) {
		return 1;
	}
	if (!test_bounded()) {
		return 1;
	}
	if (!test_pow()) {
		return 1;
	}
	if (!test_double()) {
		return 1;
	}
	return 0;
}
