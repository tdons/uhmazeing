/**
 * Fast pseudo-random number generation
 *
 * Code below is a minor adaptation of xoshiro128** [1] with the recommended
 * seeder splitmix64 [3], adaptations were made to fit the code into the style
 * of the rest of the project.  Link and source to [2] is included if we ever
 * need a 64 bit prng.
 *
 * The fast method for selecting a bounded random integer has been copied from [4].
 *
 * [1] https://prng.di.unimi.it/xoshiro128starstar.c
 * [2] https://prng.di.unimi.it/xoshiro256starstar.c       (64 bit prng, included in a comment below)
 * [3] https://prng.di.unimi.it/splitmix64.c               (32 bit prng, included in a comment below)
 * [4] https://www.pcg-random.org/posts/bounded-rands.html (included in a comment below)
 */

#include <assert.h>
#include <stdint.h>

#include "random.h"

//
// If available we prefer to use an OS provided implementation to acquire a small amount of entropy to
// seed the prng.  Otherwise we default to reading from /dev/random.
//
#if defined(__APPLE__)
#       include "TargetConditionals.h"
#       if TARGET_OS_OSX
#               include <sys/random.h>
#               define HAVE_GET_ENTROPY 1
#       endif
#elif defined(__unix__)
#       include <sys/param.h>
#       if defined(OpenBSD)
#               include <unistd.h>
#               define HAVE_GET_ENTROPY 1
#       endif
#endif

static inline uint32_t rotl(const uint32_t, int);
static uint64_t next_seed(uint64_t);

void mz_rnd_init(struct mz_rnd_t *rnd, uint64_t seed)
{
	uint64_t *s = (uint64_t*) rnd->state;
	s[0] = next_seed(seed);
	s[1] = next_seed(s[0]);
}

uint32_t mz_rnd_next32(struct mz_rnd_t *rnd)
{
	uint32_t *s = rnd->state;
	const uint32_t result = rotl(s[1] * 5, 7) * 9;
	const uint32_t t = s[1] << 9;
	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];
	s[2] ^= t;
	s[3] = rotl(s[3], 11);
	return result;
}

uint32_t mz_rnd_next(struct mz_rnd_t *rnd, uint32_t n)
{
	uint32_t x = mz_rnd_next32(rnd);
	uint64_t m = (uint64_t)x * (uint64_t)n;
	uint32_t l = (uint32_t)m;
	if (l < n) {
		uint32_t t = -n;
		if (t >= n) {
			t -= n;
			if (t >= n) {
				t %= n;
			}
		}
		while (l < t) {
			x = mz_rnd_next32(rnd);
			m = (uint64_t)x * (uint64_t)n;
			l = (uint32_t)m;
		}
	}
	return m >> 32;
}

uint32_t mz_rnd_next2pow(struct mz_rnd_t *rnd, uint8_t e)
{
	assert(((void)"exponent too large", e < 32));
	uint32_t x = mz_rnd_next32(rnd);
	uint32_t mask = ((uint32_t)1 << e) - 1;
	return x & mask;
}

double mz_rnd_nextdouble(struct mz_rnd_t *rnd)
{
	/* Copied largely from https://prng.di.unimi.it/ */ 
	uint64_t x = (((uint64_t) mz_rnd_next32(rnd)) << 32) | ((uint64_t) mz_rnd_next32(rnd));
	const union { uint64_t i; double d; } u = { .i = 0x3FFull << 52 | x >> 12 };
	return u.d - 1.0;
}

void mz_fisher_yates(struct mz_rnd_t *rnd, uint32_t *arr, uint32_t sz)
{
        for (uint32_t i = sz - 1; i > 0; --i) {
                uint32_t j = mz_rnd_next(rnd, i + 1);
                uint32_t t = arr[j];
                arr[j] = arr[i];
                arr[i] = t;
        }
}

bool mz_get_entropy(void *buf, size_t n)
{
#ifdef HAVE_GET_ENTROPY
        if (getentropy(buf, n) != 0) {
                return false;
        }
#else /* #ifdef HAVE_GET_ENTROPY */
#       warning "acquire_entropy: defaulting to reading from /dev/random: add OS specific code"
        /* Assume /dev/random is available. */
        FILE *rnd = fopen("/dev/random", "r");
        if (rnd == 0) {
                return false;
        }
        if (fread(buf, 1, n, rnd) != n) {
                return false;
        }
        fclose(rnd);
#endif /* #ifdef HAVE_GET_ENTROPY */
        return true;
}

static uint64_t next_seed(uint64_t x)
{
	uint64_t z = (x += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

static inline uint32_t rotl(const uint32_t x, int k)
{
	return (x << k) | (x >> (32 - k));
}

///*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)
//
//To the extent possible under law, the author has dedicated all copyright
//and related and neighboring rights to this software to the public domain
//worldwide. This software is distributed without any warranty.
//
//See <http://creativecommons.org/publicdomain/zero/1.0/>. */
//
//#include <stdint.h>
//
///* This is xoshiro128** 1.1, one of our 32-bit all-purpose, rock-solid
//   generators. It has excellent speed, a state size (128 bits) that is
//   large enough for mild parallelism, and it passes all tests we are aware
//   of.
//
//   Note that version 1.0 had mistakenly s[0] instead of s[1] as state
//   word passed to the scrambler.
//
//   For generating just single-precision (i.e., 32-bit) floating-point
//   numbers, xoshiro128+ is even faster.
//
//   The state must be seeded so that it is not everywhere zero. */
//
//
//static inline uint32_t rotl(const uint32_t x, int k) {
//	return (x << k) | (x >> (32 - k));
//}
//
//
//static uint32_t s[4];
//
//uint32_t next(void) {
//	const uint32_t result = rotl(s[1] * 5, 7) * 9;
//
//	const uint32_t t = s[1] << 9;
//
//	s[2] ^= s[0];
//	s[3] ^= s[1];
//	s[1] ^= s[2];
//	s[0] ^= s[3];
//
//	s[2] ^= t;
//
//	s[3] = rotl(s[3], 11);
//
//	return result;
//}
//
//
///* This is the jump function for the generator. It is equivalent
//   to 2^64 calls to next(); it can be used to generate 2^64
//   non-overlapping subsequences for parallel computations. */
//
//void jump(void) {
//	static const uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };
//
//	uint32_t s0 = 0;
//	uint32_t s1 = 0;
//	uint32_t s2 = 0;
//	uint32_t s3 = 0;
//	for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
//		for(int b = 0; b < 32; b++) {
//			if (JUMP[i] & UINT32_C(1) << b) {
//				s0 ^= s[0];
//				s1 ^= s[1];
//				s2 ^= s[2];
//				s3 ^= s[3];
//			}
//			next();	
//		}
//		
//	s[0] = s0;
//	s[1] = s1;
//	s[2] = s2;
//	s[3] = s3;
//}
//
//
///* This is the long-jump function for the generator. It is equivalent to
//   2^96 calls to next(); it can be used to generate 2^32 starting points,
//   from each of which jump() will generate 2^32 non-overlapping
//   subsequences for parallel distributed computations. */
//
//void long_jump(void) {
//	static const uint32_t LONG_JUMP[] = { 0xb523952e, 0x0b6f099f, 0xccf5a0ef, 0x1c580662 };
//
//	uint32_t s0 = 0;
//	uint32_t s1 = 0;
//	uint32_t s2 = 0;
//	uint32_t s3 = 0;
//	for(int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
//		for(int b = 0; b < 32; b++) {
//			if (LONG_JUMP[i] & UINT32_C(1) << b) {
//				s0 ^= s[0];
//				s1 ^= s[1];
//				s2 ^= s[2];
//				s3 ^= s[3];
//			}
//			next();	
//		}
//		
//	s[0] = s0;
//	s[1] = s1;
//	s[2] = s2;
//	s[3] = s3;
//}
//


///*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)
//
//To the extent possible under law, the author has dedicated all copyright
//and related and neighboring rights to this software to the public domain
//worldwide. This software is distributed without any warranty.
//
//See <http://creativecommons.org/publicdomain/zero/1.0/>. */
//
//#include <stdint.h>
//
///* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
//   generators. It has excellent (sub-ns) speed, a state (256 bits) that is
//   large enough for any parallel application, and it passes all tests we
//   are aware of.
//
//   For generating just floating-point numbers, xoshiro256+ is even faster.
//
//   The state must be seeded so that it is not everywhere zero. If you have
//   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
//   output to fill s. */
//
//static inline uint64_t rotl(const uint64_t x, int k) {
//	return (x << k) | (x >> (64 - k));
//}
//
//
//static uint64_t s[4];
//
//uint64_t next(void) {
//	const uint64_t result = rotl(s[1] * 5, 7) * 9;
//
//	const uint64_t t = s[1] << 17;
//
//	s[2] ^= s[0];
//	s[3] ^= s[1];
//	s[1] ^= s[2];
//	s[0] ^= s[3];
//
//	s[2] ^= t;
//
//	s[3] = rotl(s[3], 45);
//
//	return result;
//}
//
//
///* This is the jump function for the generator. It is equivalent
//   to 2^128 calls to next(); it can be used to generate 2^128
//   non-overlapping subsequences for parallel computations. */
//
//void jump(void) {
//	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };
//
//	uint64_t s0 = 0;
//	uint64_t s1 = 0;
//	uint64_t s2 = 0;
//	uint64_t s3 = 0;
//	for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
//		for(int b = 0; b < 64; b++) {
//			if (JUMP[i] & UINT64_C(1) << b) {
//				s0 ^= s[0];
//				s1 ^= s[1];
//				s2 ^= s[2];
//				s3 ^= s[3];
//			}
//			next();	
//		}
//		
//	s[0] = s0;
//	s[1] = s1;
//	s[2] = s2;
//	s[3] = s3;
//}
//
//
//
///* This is the long-jump function for the generator. It is equivalent to
//   2^192 calls to next(); it can be used to generate 2^64 starting points,
//   from each of which jump() will generate 2^64 non-overlapping
//   subsequences for parallel distributed computations. */
//
//void long_jump(void) {
//	static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };
//
//	uint64_t s0 = 0;
//	uint64_t s1 = 0;
//	uint64_t s2 = 0;
//	uint64_t s3 = 0;
//	for(int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
//		for(int b = 0; b < 64; b++) {
//			if (LONG_JUMP[i] & UINT64_C(1) << b) {
//				s0 ^= s[0];
//				s1 ^= s[1];
//				s2 ^= s[2];
//				s3 ^= s[3];
//			}
//			next();	
//		}
//		
//	s[0] = s0;
//	s[1] = s1;
//	s[2] = s2;
//	s[3] = s3;
//}


///*  Written in 2015 by Sebastiano Vigna (vigna@acm.org)
//
//To the extent possible under law, the author has dedicated all copyright
//and related and neighboring rights to this software to the public domain
//worldwide. This software is distributed without any warranty.
//
//See <http://creativecommons.org/publicdomain/zero/1.0/>. */
//
//#include <stdint.h>
//
///* This is a fixed-increment version of Java 8's SplittableRandom generator
//   See http://dx.doi.org/10.1145/2714064.2660195 and 
//   http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html
//
//   It is a very fast generator passing BigCrush, and it can be useful if
//   for some reason you absolutely want 64 bits of state. */
//
//static uint64_t x; /* The state can be seeded with any value. */
//
//uint64_t next() {
//	uint64_t z = (x += 0x9e3779b97f4a7c15);
//	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
//	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
//	return z ^ (z >> 31);
//}


//// From: https://www.pcg-random.org/posts/bounded-rands.html
//// The fastest (unbiased) method is Lemire's (with an extra tweak from me). Here it is:
//uint32_t bounded_rand(rng_t& rng, uint32_t range) {
//    uint32_t x = rng();
//    uint64_t m = uint64_t(x) * uint64_t(range);
//    uint32_t l = uint32_t(m);
//    if (l < range) {
//        uint32_t t = -range;
//        if (t >= range) {
//            t -= range;
//            if (t >= range) 
//                t %= range;
//        }
//        while (l < t) {
//            x = rng();
//            m = uint64_t(x) * uint64_t(range);
//            l = uint32_t(m);
//        }
//    }
//    return m >> 32;
//}
