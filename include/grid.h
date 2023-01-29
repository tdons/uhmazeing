#ifndef MZ_GRID_H
#define MZ_GRID_H

#include <stdbool.h>
#include <stdint.h>

enum mz_wall_t { north, east, south, west, };

struct mz_grid_t {
        uint8_t *grid;
        uint32_t H, W, sz;
};

/**
 * Create a 2D grid of cells
 *
 * @param uint32_t height 
 * @param uint32_t width 
 */
struct mz_grid_t *mz_grid_create(uint32_t, uint32_t);

void mz_grid_remove_wall1(struct mz_grid_t *, uint32_t, enum mz_wall_t);
void mz_grid_remove_wall(struct mz_grid_t *, uint32_t, uint32_t, enum mz_wall_t);

bool mz_grid_has_wall1(const struct mz_grid_t *, uint32_t, enum mz_wall_t);
bool mz_grid_has_wall(const struct mz_grid_t *, uint32_t, uint32_t, enum mz_wall_t);

/**
 * Destroy a grid
 */
void mz_grid_destroy(struct mz_grid_t *);

#endif /* #ifndef MZ_GRID_H */
