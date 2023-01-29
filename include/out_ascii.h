#ifndef MZ_OUT_ASCII_H
#define MZ_OUT_ASCII_H

#include "grid.h"
#include "path.h"

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
void mz_out_ascii_simple(const struct mz_grid_t *, const struct mz_path_t *);

void mz_out_ascii_boxchars(const struct mz_grid_t *, const struct mz_path_t *);

/**
 * Example output (8x4)
 * ┌─────────┬─────┐ 
 * │ ╷ ┌─┐ ╶─┼───╴ │ 
 * ├─┘ │ ╵ ╶─┘ ╶─┐ │ 
 * ├───┘ ╶───┐ ┌─┘ │ 
 * └─────────┴─┴───┘
 */
void mz_out_ascii_boxchars_compact(const struct mz_grid_t *);

#endif /* #ifndef MZ_OUT_ASCII_H */
