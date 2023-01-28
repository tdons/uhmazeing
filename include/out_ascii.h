#ifndef MZN_OUT_ASCII_H
#define MZN_OUT_ASCII_H

#include "grid.h"

/**
 * Example out (8x4)
 * +---+---+---+---+---+---+---+---+
 * |                   |           |
 * +   +   +---+   +---+---+---+   +
 * |   |   |   |       |           |
 * +---+   +   +   +---+   +---+   +
 * |       |                   |   |
 * +---+---+   +---+---+   +---+   +
 * |                   |   |       |
 * +---+---+---+---+---+---+---+---+
 */
void mzn_out_ascii_simple(const struct mzn_grid_t *);

void mzn_out_ascii_boxchars(const struct mzn_grid_t *);

/**
 * Example output (8x4)
 * ┌─────────┬─────┐ 
 * │ ╷ ┌─┐ ╶─┼───╴ │ 
 * ├─┘ │ ╵ ╶─┘ ╶─┐ │ 
 * ├───┘ ╶───┐ ┌─┘ │ 
 * └─────────┴─┴───┘
 */
void mzn_out_ascii_boxchars_compact(const struct mzn_grid_t *);

#endif /* #ifndef MZN_OUT_ASCII_H */
