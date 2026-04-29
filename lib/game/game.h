#ifndef GAME_H
#define GAME_H
#include <stdint.h>

const int MAX_OBJECTS = 100;

typedef enum { OBJ_BLOCK } ObjType;

typedef struct {
  int16_t x;
  int16_t y;
  ObjType type;
} Obj;

typedef enum { LEFT, RIGHT, CENTER } Direction;

typedef struct {
  struct {
    int16_t x;
    int16_t y;
    int8_t y_vel; // velocity
    bool on_ground;
    Direction facing;
  } player;

  bool sw_prev_pressed;

  int16_t camera_x;
  int16_t camera_y;

  Obj objects[MAX_OBJECTS];
  int object_count;
} GameState;

typedef struct {
  int8_t x_move;
  bool sw_pressed;
} Inputs;

void GameState_init(GameState *s);
void GameState_update(GameState *s, Inputs *inputs);

#endif
