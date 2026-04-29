#include "world.h"

typedef enum {
  TILE_PLAYER = 'P',
  TILE_BLOCK = '#',
  TILE_SPACE = '.',
  TILE_FLAG = 'F',
  TILE_SPIKE = '*',
} TILE;

static const int16_t WORLD_HEIGHT = 5;
static const int16_t WORLD_WIDTH = 64;

static const char world[WORLD_HEIGHT][WORLD_WIDTH] = {
    "#.............................#...............................#",
    "#.............................#...............................#",
    "#............................##...............................#",
    "#.....#...####..............#.................................#",
    "#P.......#F.*..#....###....#..................................#",
};

static char world_get(int16_t x, int16_t y);
static void add_obj(GameState *s, ObjType type, int16_t x, int16_t y);

void load_world(GameState *s) {
  char c;
  int16_t x, y;
  for (y = 0; y < WORLD_HEIGHT; y++) {
    for (x = 0; x < WORLD_WIDTH; x++) {
      if (s->object_count + 1 >= MAX_OBJECTS)
        goto done;

      switch ((c = world_get(x, y))) {
      case TILE_SPACE:
        continue;
      case TILE_PLAYER:
        s->player.y = y;
        s->player.x = x;
        break;
      case TILE_BLOCK:
        add_obj(s, OBJ_BLOCK, x, y);
        break;
      case TILE_FLAG:
        add_obj(s, OBJ_FLAG, x, y);
        break;
      case TILE_SPIKE:
        add_obj(s, OBJ_SPIKE, x, y);
        break;
      }
    }
  }

done:
  return;
}

/* helpers */
static char world_get(int16_t x, int16_t y) {
  if (x < 0 || y < 0 || y > WORLD_HEIGHT - 1 || x > WORLD_WIDTH - 1)
    return '.';

  // bottom left of the world is (0, 0)
  return world[WORLD_HEIGHT - y - 1][x];
}

static void add_obj(GameState *s, ObjType type, int16_t x, int16_t y) {
  s->objects[s->object_count++] = {.x = x, .y = y, .type = type};
}
