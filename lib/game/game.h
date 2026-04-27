#ifndef GAME_H
#define GAME_H
#include <stdint.h>

typedef enum { LEFT, RIGHT, STOP } Direction;

typedef struct {
  uint16_t player_x;
  uint16_t player_y;
  Direction player_facing;
} GameState;

typedef struct {
  int8_t move_x;
} Inputs;

void GameState_init(GameState *s);
void GameState_update(GameState *s, Inputs *inputs);

#endif
