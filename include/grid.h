#ifndef MZN_GRID_H
#define MZN_GRID_H

#include <stdbool.h>
#include <stdint.h>

enum mzn_wall_t { north, east, south, west, };

struct mzn_grid_t {
        uint8_t *grid;
        uint32_t H, W, sz;
};

/**
 * Create a 2D grid of cells
 *
 * @param uint32_t height 
 * @param uint32_t width 
 */
struct mzn_grid_t *mzn_grid_create(uint32_t, uint32_t);

void mzn_grid_remove_wall1(struct mzn_grid_t *, uint32_t, enum mzn_wall_t);
void mzn_grid_remove_wall(struct mzn_grid_t *, uint32_t, uint32_t, enum mzn_wall_t);

bool mzn_grid_has_wall1(const struct mzn_grid_t *, uint32_t, enum mzn_wall_t);
bool mzn_grid_has_wall(const struct mzn_grid_t *, uint32_t, uint32_t, enum mzn_wall_t);

/**
 * Destroy a grid
 */
void mzn_grid_destroy(struct mzn_grid_t *);

#endif /* #ifndef MZN_GRID_H */
