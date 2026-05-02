#ifndef GAME_H
#define GAME_H
#include "input_types.h"
#include <stdint.h>

typedef enum {
  GAME_STATUS_PLAYING,
  GAME_STATUS_LOST,
  GAME_STATUS_WON,
  GAME_STATUS_RESET
} GameStatus;

typedef enum { TILE_EMPTY, TILE_BLOCK, TILE_SPIKE, TILE_FLAG } Tile;

const int MAX_MAP_WIDTH = 32;
const int MAX_MAP_HEIGHT = 8;

const int MAX_CAM_X = MAX_MAP_WIDTH - 17;
const int MAX_CAM_Y = MAX_MAP_HEIGHT - 2;

// note the width comes before the height to match x, y coord convention
typedef Tile TileMap[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

typedef enum { LEFT, RIGHT, CENTER } Direction;

typedef enum {
  JUMP_NONE,
  JUMP_START,
  JUMP_RISING,
  JUMP_APEX,
  JUMP_FALLING,
} JumpPhase;

typedef struct {
  GameStatus game_status;
  TileMap map;

  struct {
    int16_t x;
    int16_t y;
    JumpPhase jump_phase;
    bool on_ground;
    Direction facing;
  } player;

  int16_t camera_x;
  int16_t camera_y;
} GameState;

void GameState_init(GameState *s);
void GameState_update(GameState *s, GameInputs *inputs);

#endif
