#include <assert.h>

#include "generate.h"
#include "grid.h"
#include "union_find.h"
#include "permutation.h"
#include "random.h"

void mzn_kruskal(struct mzn_rnd_t *rnd, struct mzn_grid_t *g)
{
	uint32_t sz = g->H * g->W * 2;
	struct mzn_uf_t *uf = mzn_uf_create(g->H * g->W);
	uint32_t *walls = malloc(sizeof(uint32_t) * sz);
	for (uint32_t i = 0; i < sz; i++) {
		walls[i] = i;
	}
	mzn_fisher_yates(rnd, walls, sz);  // Randomize the list of walls.
	uint32_t c, wall, x1, y1, x2, y2;
	for (uint32_t i = 0; i < sz; i++) {
		c = walls[i];
		wall = c % 2; // north = 0, west = 1
		c = (c - wall) / 2;
		x1 = c % g->W;
		y1 = (c - x1) / g->W;
		if ((y1 == 0 && wall == 0) || (x1 == 0 && wall == 1)) {
			continue; // Skip north walls for top row, west walls for left column.
		}
		x2 = x1 - (wall == 1 ? 1 : 0);
		y2 = y1 - (wall == 0 ? 1 : 0);
		if (mzn_uf_find(uf, y1 * g->W + x1) == mzn_uf_find(uf, y2 * g->W + x2)) {
			continue; // There already exists a path between cells, stop.
		}
		mzn_grid_remove_wall(g, y1, x1, wall == 0 ? north : west); // Create path between cells.
		mzn_uf_union(uf, y1 * g->W + x1, y2 * g->W + x2);
	}
	free(walls);
	mzn_uf_destroy(uf);
}

void mzn_dfs(struct mzn_rnd_t *rnd, struct mzn_grid_t *g)
{
	struct pos_t { uint32_t y; uint32_t x; };

	/*
	 * We want to visit the four neighbours of a cell in random order many times during the
	 * search, to make this operation as cheap as possible we generate all 4! permutations
	 * of [0, 1, 2, 3].  During the search we can select one of these at random simply by
	 * generating a random number [0, 4!).  This is (probably) cheaper than the alternative of
	 * shuffling a list of [0, 1, 2, 3] each time we need to randomly visit neighbours.
	 */
	struct mzn_perms_t *ps = mzn_perms_ofseq0to(4);
	struct pos_t *stack = malloc(sizeof(*stack) * g->W * g->H);
	bool *visited = calloc(sizeof(*visited), g->W * g->H);
	uint32_t perm;
	struct pos_t c, n;
	enum mzn_wall_t d;
	stack[0].y = mzn_rnd_next(rnd, g->H);
	stack[0].x = mzn_rnd_next(rnd, g->W);
	uint32_t i = 1;
	while(i > 0) {
		c = stack[--i];
		visited[c.y * g->W + c.x] = true;
		perm = mzn_rnd_next(rnd, ps->sz);
		for (uint8_t j = 0; j < 4; j++) {
			n = c;
			d = ps->perms[perm * ps->n + j];
			switch (d) {
			case north:
				if (n.y == 0) { continue; }
				n.y--;
				break;
			case east:
				if (n.x >= g->W - 1) { continue; }
				n.x++;
				break;
			case south:
				if (n.y >= g->H - 1) { continue; }
				n.y++;
				break;
			case west:
				if (n.x == 0) { continue; }
				n.x--;
				break;
			}
			if (visited[n.y * g->W + n.x]) {
				continue;
			}
			mzn_grid_remove_wall(g, c.y, c.x, d);
			stack[i++] = c;
			stack[i++] = n;
			break;
		}
	}
	free(visited);
	free(stack);
	mzn_perms_destroy(ps);
}

