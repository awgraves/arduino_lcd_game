#include "world.h"

// these must be smaller than (or equal to) the max defs in game.h!
static const int16_t WORLD_HEIGHT = 8;
static const int16_t WORLD_WIDTH = 32;

static const char world[WORLD_HEIGHT][WORLD_WIDTH] = {
    "#.............................#", //
    "#.............................#", //
    "#.............................#", //
    "#..................#**........#", //
    "#.................##....####..#", //
    "#................##..........##", //
    "#.....#...##..#*##..........###", //
    "#P..#...#....#F....#.***#######", //
};

static char world_get(int16_t x, int16_t y);

void load_world(GameState *s) {
  char c;
  int16_t x, y;

  // note! inverting the heights and widths when setting tiles
  // so that they can accessed more intuitively as coords (x first, then y)
  for (y = 0; y < WORLD_HEIGHT; y++) {
    for (x = 0; x < WORLD_WIDTH; x++) {
      switch ((c = world_get(x, y))) {
      case '.':
        s->map[x][y] = TILE_EMPTY;
        break;
      case 'P':
        s->player.x = x;
        s->player.y = y;
        s->map[x][y] = TILE_EMPTY;
        break;
      case '#':
        s->map[x][y] = TILE_BLOCK;
        break;
      case 'F':
        s->map[x][y] = TILE_FLAG;
        break;
      case '*':
        s->map[x][y] = TILE_SPIKE;
        break;
      }
    }
  }
}

/* helpers */
static char world_get(int16_t x, int16_t y) {
  // safeguard against accidental out of bounds read
  if (x < 0 || y < 0 || y > WORLD_HEIGHT - 1 || x > WORLD_WIDTH - 1)
    return '.';

  // bottom left of the world is (0, 0)
  return world[WORLD_HEIGHT - y - 1][x];
}
