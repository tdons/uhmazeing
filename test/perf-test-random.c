#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>

#include "../src/random.c"

static uint32_t r(size_t n) { return rand() / (RAND_MAX / n + 1); }


int main()
{
	#define START() gettimeofday(&tval_before, 0);
	#define STOP() do { \
		gettimeofday(&tval_after, 0); \
		timersub(&tval_after, &tval_before, &tval_result); \
		printf("%ld.%06d\n", tval_result.tv_sec, tval_result.tv_usec); \
	} while (0);

	struct timeval tval_before, tval_after, tval_result;
	struct mz_rnd_t rnd;
	uint32_t iters;
	__unused uint32_t unused;
	mz_rnd_init(&rnd, 0);

	printf("%u iterations of:\n", (iters = 1 << 27));
	printf("%30s", "stdlib rand()... ");
	START();
	for (uint32_t i = 0; i < iters; i++) {
		unused = rand();
	}
	STOP();

	printf("%30s", "xoshiro128... ");
	START();
	for (uint32_t i = 0; i < iters; i++) {
		unused = mz_rnd_next32(&rnd);
	}
	STOP();
	
	printf("%u iterations of:\n", 1024 * (iters = 1 << 17));
	printf("%30s", "stdlib bounded rand()... ");
	START();
	for (uint32_t n = 1; n < 1024; n++) {
		for (uint32_t i = 0; i < iters; i++) {
			unused = r(n);
		}
	}
	STOP();

	printf("%30s", "xoshiro128 bounded rand()... ");
	START();
	for (uint32_t n = 1; n < 1024; n++) {
		for (uint32_t i = 0; i < iters; i++) {
			unused = mz_rnd_next(&rnd, n);
		}
	}
	STOP();

	// Prevent optimizer from removing the final loop.
	fprintf(fopen("/dev/null", "w"), "%u %u %u %u", rnd.state[0], rnd.state[1], rnd.state[2], rnd.state[3]);

	return 0;
}
