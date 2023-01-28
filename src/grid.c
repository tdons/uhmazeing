#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "grid.h"

/*
 * A grid of H x W cells, cells indexed by (y, x) with 0 <= y && y < H && 0 <= x && x < W.
 *
 * For every cell (y, x) we keep track of two things:
 * - does the cell have a north border?
 * - does the cell have a west border?
 * +----------+----------+
 * |          |          |
 * |   y, x   | y, x + 1 |
 * |          |          |
 * +----------+----------+
 * |          |
 * | y + 1, x |
 * |          |
 * +----------+
 *
 * Deriving if a cell has an east and/or south borders is done by inspecting resp. the west and north border of its neighbours:
 * - cell (y, x) has an east border iff cell (y, x + 1) has a west border.
 * - cell (y, x) has a south border iff cell (y + 1, x) has a north border.
 *
 * This is the reason that the dimensions of the 2D cells array are one larger than H and W: the final row and column
 * in the array are 'fake' cells that we require to keep track of the southern and eastern borders of the grid.
 */

#define NORTH_BIT (1 << 0)
#define WEST_BIT  (1 << 1)

#define __fallthrough __attribute__((fallthrough))

struct mzn_grid_t *mzn_grid_create(uint32_t H, uint32_t W)
{
	struct mzn_grid_t *g = malloc(sizeof(*g));
	g->grid = calloc((H + 1) * (W + 1) - 1, sizeof(*g->grid));
	g->H = H;
	g->W = W;
	g->sz = H * W;
	return g;
}

void mzn_grid_remove_wall(struct mzn_grid_t *g, uint32_t y, uint32_t x, enum mzn_wall_t c)
{
	assert(((void)"out of bounds", y < g->H && x < g->W));
	uint32_t bit;
	switch(c) {
		case south:
			++y;
			__fallthrough;
		case north:
			bit = NORTH_BIT;
			break;
		case east:
			++x;
			__fallthrough;
		case west:
			bit = WEST_BIT;
			break;
	}
	g->grid[y * (g->W + 1) + x] |= bit;
}

bool mzn_grid_has_wall(const struct mzn_grid_t *g, uint32_t y, uint32_t x, enum mzn_wall_t c)
{
	assert(((void)"out of bounds", y < g->H && x < g->W));
	uint32_t bit;
	switch(c) {
		case south:
			++y;
			__fallthrough;
		case north:
			bit = NORTH_BIT;
			break;
		case east:
			++x;
			__fallthrough;
		case west:
			bit = WEST_BIT;
			break;
	}
	return (g->grid[y * (g->W + 1) + x] & bit) == 0;
}

void mzn_grid_destroy(struct mzn_grid_t *g)
{
	free(g->grid);
	free(g);
}

