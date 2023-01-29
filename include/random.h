#ifndef MZ_RANDOM_H
#define MZ_RANDOM_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct mz_rnd_t {
        uint32_t state[4];
};

/**
 * Initialize mz_rnd_t with the provided 64 bit seed.
 *
 * @param uint64_t seed the seed value for the prng
 */
void mz_rnd_init(struct mz_rnd_t *, uint64_t);

/**
 * @return a pseudo-random integer [0,2^32)
 */
uint32_t mz_rnd_next32(struct mz_rnd_t *);

/**
 * @param uint32_t n exclusive upper-bound
 * @return a pseudo-random integer [0,n)
 */
uint32_t mz_rnd_next(struct mz_rnd_t *, uint32_t);

/**
 * @param uint8_t e the exponent
 * @return a pseudo-random integer [0,2^e)
 */
uint32_t mz_rnd_next2pow(struct mz_rnd_t *, uint8_t);

/**
 * @return a double in the unit interval [0,1)
 */
double mz_rnd_nextdouble(struct mz_rnd_t *);

/**
 * @param uint32_t *arr the array to shuffle
 * @param uint32_t sz the size of the array to shuffle
 * Fisher-Yates shuffle
 */
void mz_fisher_yates(struct mz_rnd_t *, uint32_t *, uint32_t);

/**
 * @param void * where to write the random bytes
 * @param uint32_t number of random bytes to write 
 */
bool mz_get_entropy(void *, size_t);

#endif /* #ifndef MZ_RANDOM_H */
