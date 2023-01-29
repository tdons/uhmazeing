#include <stdio.h>
#include <stdlib.h>

#include "grid.h"

/**
 * boxchars[                     8, 9, 10, 11, 12, 13, 14, 15] = north (bit 4 set)
 * boxchars[         4, 5, 6, 7,               12, 13, 14, 15] = west  (bit 3 set)
 * boxchars[   2, 3,       6, 7,       10, 11,         14, 15] = south (bit 2 set)
 * boxchars[1,    3,    5,    7,    9,     11,     13,     15] = east  (bit 1 set)
 *
 * (ASCII in this filename is a bit of a misnomer because the stuff below is utf-8.  Oh well.)
 */
static const char *boxchars[] = {
	" ", "╶", "╷", "┌",
	"╴", "─", "┐", "┬",
	"╵", "└", "│", "├",
	"┘", "┴", "┤", "┼",
};
#define HORIZONTAL_INDEX 5
#define VERTICAL_INDEX 10

void mz_out_ascii_simple(const struct mz_grid_t *g)
{
	for (uint32_t y = 0; y < g->H; y++) {
		printf("+");
		for (uint32_t x = 0; x < g->W; x++) {  /* Top of cell */
			printf(mz_grid_has_wall(g, y, x, north) ? "---+" : "   +");
		}
		printf("\n%c", mz_grid_has_wall(g, y, 0, west) ? '|' : ' ');
		for (uint32_t x = 0; x < g->W; x++) {  /* Interior */
			printf("   %c", mz_grid_has_wall(g, y, x, east) ? '|' : ' ');
		}
		printf("\n");
	}
	printf("+");
	for (uint32_t x = 0; x < g->W; x++) {  /* South wall */
		printf(mz_grid_has_wall(g, g->H - 1, x, south) ? "---+" : "   +");
	}
	printf("\n");
}

void mz_out_ascii_boxchars(const struct mz_grid_t *g)
{
	int n, e, s, w;
	bool y0, x0, xW, yH;
	for (uint32_t y = 0; y <= g->H; y++) {
		y0 = y > 0, yH = y < g->H;
		for (uint32_t x = 0; x <= g->W; x++) {  /* Top of cell */
			x0 = x > 0, xW = x < g->W;
			n = (y0 && x0             && mz_grid_has_wall(g, y - 1, x - 1,  east)) ||
			    (y0             && xW && mz_grid_has_wall(g, y - 1,     x,  west)) ? 8 : 0;
			w = (y0 && x0             && mz_grid_has_wall(g, y - 1, x - 1, south)) ||
			    (      x0 && yH       && mz_grid_has_wall(g,     y, x - 1, north)) ? 4 : 0;
			s = (      x0 && yH       && mz_grid_has_wall(g,     y, x - 1,  east)) ||
			    (            yH && xW && mz_grid_has_wall(g,     y,     x,  west)) ? 2 : 0;
			e = (y0             && xW && mz_grid_has_wall(g, y - 1,     x, south)) ||
			    (            yH && xW && mz_grid_has_wall(g,     y,     x, north)) ? 1 : 0;
			printf("%s%s%s%s", boxchars[n + w + s + e],
				boxchars[e != 0 ? HORIZONTAL_INDEX : 0],
				boxchars[e != 0 ? HORIZONTAL_INDEX : 0],
				boxchars[e != 0 ? HORIZONTAL_INDEX : 0]
			);
		}
		if (y != g->H) {
			printf("\n");
			for (uint32_t x = 0; x <= g->W; x++) {  /* Interior */
				x0 = x > 0, xW = x < g->W;
				n = (y0 && x0             && mz_grid_has_wall(g, y - 1, x - 1,  east)) ||
				    (y0             && xW && mz_grid_has_wall(g, y - 1,     x,  west)) ? 8 : 0;
				w = (y0 && x0             && mz_grid_has_wall(g, y - 1, x - 1, south)) ||
				    (      x0 && yH       && mz_grid_has_wall(g,     y, x - 1, north)) ? 4 : 0;
				s = (      x0 && yH       && mz_grid_has_wall(g,     y, x - 1,  east)) ||
				    (            yH && xW && mz_grid_has_wall(g,     y,     x,  west)) ? 2 : 0;
				e = (y0             && xW && mz_grid_has_wall(g, y - 1,     x, south)) ||
				    (            yH && xW && mz_grid_has_wall(g,     y,     x, north)) ? 1 : 0;
				printf("%s", boxchars[s != 0 ? VERTICAL_INDEX : 0]);
				printf("   ");
			}
		}
		printf("\n");
	}
}

void mz_out_ascii_boxchars_compact(const struct mz_grid_t *g)
{
	int n, e, s, w;
	bool y0, x0, xW, yH;
	for (uint32_t y = 0; y <= g->H; y++) {
		for (uint32_t x = 0; (uint32_t)x <= g->W; x++) {
			y0 = y > 0, x0 = x > 0, xW = x < g->W, yH = y < g->H;
			n = (y0 && x0             && mz_grid_has_wall(g, y - 1, x - 1,  east)) ||
			    (y0             && xW && mz_grid_has_wall(g, y - 1,     x,  west)) ? 8 : 0;
			w = (y0 && x0             && mz_grid_has_wall(g, y - 1, x - 1, south)) ||
			    (      x0 && yH       && mz_grid_has_wall(g,     y, x - 1, north)) ? 4 : 0;
			s = (      x0 && yH       && mz_grid_has_wall(g,     y, x - 1,  east)) ||
			    (            yH && xW && mz_grid_has_wall(g,     y,     x,  west)) ? 2 : 0;
			e = (y0             && xW && mz_grid_has_wall(g, y - 1,     x, south)) ||
			    (            yH && xW && mz_grid_has_wall(g,     y,     x, north)) ? 1 : 0;
			printf("%s%s", boxchars[n + w + s + e], boxchars[e == 0 ? 0 : HORIZONTAL_INDEX]);
		}
		printf("\n");
	}
}
