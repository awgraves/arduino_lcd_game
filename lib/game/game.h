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

typedef enum {
  JUMP_NONE,
  JUMP_START,
  JUMP_RISING,
  JUMP_APEX,
  JUMP_FALLING,
} JumpPhase;

typedef struct {
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

typedef enum {
  BTN_OPEN,
  BTN_JUST_PRESSED,
  BTN_HOLD,
  BTN_JUST_RELEASED
} ButtonState;

typedef struct {
  int8_t x_move;
  ButtonState btn_state;
} Inputs;

void GameState_init(GameState *s);
void GameState_update(GameState *s, Inputs *inputs);

#endif
