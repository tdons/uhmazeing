#ifndef MZ_PERMS_T
#define MZ_PERMS_T

struct mz_perms_t {
        uint8_t *perms;
        uint8_t n;
        uint32_t sz;
};

/**
 * Generate a list of all the permutations of the sequence [0, 1, ..., n)
 *
 * The list will have length n! and therefore n is limited to 12 (since 13! > 2^32-1)
 *
 * Note: generating 12! permutations takes around 9~ seconds and takes about 5.4 GiB memory.
 *       12! = 479001600, 12!*12/1024^3 ≈ 5.35 GiB
 */
struct mz_perms_t *mz_perms_ofseq0to(uint8_t);

/**
 * Free memory.
 */
void mz_perms_destroy(struct mz_perms_t *);

#endif /* #ifndef MZ_PERMS_T */
