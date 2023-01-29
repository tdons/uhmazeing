#ifndef MZ_PATH_H
#define MZ_PATH_H

#include <stdbool.h>
#include <stdint.h>

struct mz_path_t;

struct mz_path_t *mz_dial(struct mz_grid_t *);

uint32_t mz_dist(const struct mz_path_t *, uint32_t, uint32_t);

bool mz_on_path(const struct mz_path_t *, uint32_t, uint32_t);

uint32_t mz_prev_y(const struct mz_path_t *, uint32_t, uint32_t);

uint32_t mz_prev_x(const struct mz_path_t *, uint32_t, uint32_t);

uint32_t mz_path_length(const struct mz_path_t *);

void mz_path_destroy(struct mz_path_t *);

#endif /* #ifndef MZ_PATH_H */
