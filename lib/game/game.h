#ifndef GAME_H
#define GAME_H
#include <stdint.h>

const int MAX_OBJECTS = 100;

typedef enum { TILE_EMPTY, TILE_BLOCK, TILE_SPIKE, TILE_FLAG } Tile;

const int MAX_MAP_WIDTH = 64;
const int MAX_MAP_HEIGHT = 10;

// note the width comes before the height to match x, y coord convention
typedef Tile TileMap[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

typedef enum { LEFT, RIGHT, CENTER } Direction;

typedef struct {
  TileMap map;

  struct {
    int16_t x;
    int16_t y;
    int8_t y_vel;
    int8_t y_hold_remaining;
    bool on_ground;
    Direction facing;
  } player;

  bool sw_prev_pressed;

  int16_t camera_x;
  int16_t camera_y;
} GameState;

typedef struct {
  int8_t x_move;
  bool sw_pressed;
} Inputs;

void GameState_init(GameState *s);
void GameState_update(GameState *s, Inputs *inputs);

#endif
