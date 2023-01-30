#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "grid.h"
#include "path.h"
#include "pq.h"

struct mz_path_t {
	struct mz_grid_t *grid;
	uint32_t *dist;
	uint32_t *prev;
	bool* on_shortest_path;
};


/**
 * Specialized variant of Dijkstra for graphs with bounded edge weight.
 *
 * Our graph is a grid and hence the edge weight is always 1.  This permits
 * us to use a monotone priority queue instead of a regular pq and thus
 * a decreased worst-case performance of of O(|E| + |V|) instead of
 * the usual O(|E| + |V| log |V|) with a fib heap.
 */
struct mz_path_t *mz_dial(struct mz_grid_t *g)
{
	struct mz_path_t *p = malloc(sizeof(*p));
	p->grid = g;
	p->dist = malloc(sizeof(*p->dist) * g->sz);
	p->prev = calloc(sizeof(*p->dist), g->sz);
	p->on_shortest_path = calloc(sizeof(*p->on_shortest_path), g->sz);
	struct mz_mpq_t *pq = mz_mpq_create_monotone(g->sz);

	memset(p->dist, UINT32_MAX, sizeof(*p->dist) * g->sz);

	p->dist[0] = 0;
	mz_mpq_insert(pq, 0, 0);
	uint64_t c, n, cy, cx;
	int32_t dy, dx;
	uint32_t d;
	while (!mz_mpq_empty(pq)) {
		c = mz_mpq_pop(pq);
		cx = c % g->W;
		cy = (c - cx) / g->W;
		d = p->dist[c];
		for (int i = 0; i < 4; i++) {
			if (mz_grid_has_wall(g, cy, cx, i)) {
				continue;
			}
			dy = dx = 0;
                        switch (i) {
                        case north:
                                if (cy == 0) { continue; }
				dy = -1;
                                break;
                        case east:
                                if (cx >= g->W - 1) { continue; }
				dx = 1;
                                break;
                        case south:
                                if (cy >= g->H - 1) { continue; }
				dy = 1;
                                break;
                        case west:
                                if (cx == 0) { continue; }
				dx = -1;
                                break;
                        }
			n = (cy + dy) * g->W + (cx + dx);
			if (p->dist[n] <= d) {
				continue;
			}
			p->dist[n] = d + 1;
			p->prev[n] = c;
			mz_mpq_insert(pq, p->dist[n], n);
		}
	}

	uint32_t y = g->H - 1, x = g->W - 1, t;
	do {
		p->on_shortest_path[y * g->W + x] = true;
		t = mz_prev_y(p, y, x);
		x = mz_prev_x(p, y, x);
		y = t;
	} while (!(y == 0 && x == 0));
	p->on_shortest_path[0] = true;

	mz_mpq_destroy(pq);
	return p;
}

uint32_t mz_dist(const struct mz_path_t *p, uint32_t y, uint32_t x)
{
	return p->dist[y * p->grid->W + x];
}

bool mz_on_path(const struct mz_path_t *p, uint32_t y, uint32_t x)
{
	return p->on_shortest_path[y * p->grid->W + x];
}

uint32_t mz_prev_y(const struct mz_path_t *p, uint32_t y, uint32_t x)
{
	uint32_t c = p->prev[y * p->grid->W + x];
	x = c % p->grid->W;
	y = (c - x) / p->grid->W;
	return y;
}

uint32_t mz_prev_x(const struct mz_path_t *p, uint32_t y, uint32_t x)
{
	uint32_t c = p->prev[y * p->grid->W + x];
	x = c % p->grid->W;
	y = (c - x) / p->grid->W;
	return x;
}

uint32_t mz_path_length(const struct mz_path_t *p)
{
	uint32_t y = p->grid->H - 1, x = p->grid->W - 1;
	return p->dist[y * p->grid->W + x];
}

void mz_path_destroy(struct mz_path_t *p)
{
	free(p->dist);
	free(p->prev);
	free(p->on_shortest_path);
	free(p);
}
