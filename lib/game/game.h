#ifndef GAME_H
#define GAME_H
#include <stdint.h>

const int MAX_OBJECTS = 10;

typedef enum { NONE, BLOCK } ObjType;

typedef struct {
  ObjType type;
  uint16_t pos_x;
  uint16_t pos_y;
} Obj;

typedef enum { LEFT, RIGHT, STOP } Direction;

typedef struct {
  uint16_t player_x;
  uint16_t player_y;
  Direction player_facing;

  Obj objects[MAX_OBJECTS];
  int object_count;
} GameState;

typedef struct {
  int8_t move_x;
} Inputs;

void GameState_init(GameState *s);
void GameState_update(GameState *s, Inputs *inputs);

#endif
