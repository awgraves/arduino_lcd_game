#include "game.h"
#include <Arduino.h>

/* Public API */
void GameState_init(GameState *s) {
  s->player_x = 7;
  s->player_y = 1;
  s->player_facing = STOP;

  for (int i = 0; i < MAX_OBJECTS; i++)
    s->objects[i] = {.type = NONE};

  // first block
  s->objects[0] = {.type = BLOCK, .pos_x = 0, .pos_y = 0};
  s->objects[1] = {.type = BLOCK, .pos_x = 0, .pos_y = 1};
  s->objects[2] = {.type = BLOCK, .pos_x = 12, .pos_y = 1};
  s->objects[3] = {.type = BLOCK, .pos_x = 13, .pos_y = 1};
  s->object_count = 4;
};

static void update_player_pos(GameState *s, int8_t x);

void GameState_update(GameState *s, Inputs *in) {
  if (in->move_x != 0) {
    s->player_facing = (in->move_x == 1) ? RIGHT : LEFT;
    // temp logic
    update_player_pos(s, in->move_x);
  } else {
    s->player_facing = STOP;
  }
}

/* helpers */

static void update_player_pos(GameState *s, int8_t move_x) {
  // guard against negative
  if (move_x < 0 && s->player_x == 0)
    return;

  uint16_t potential = s->player_x + move_x;

  for (int i = 0; i < s->object_count; i++) {
    Obj *obj = &s->objects[i];
    if (obj->type == BLOCK && potential == obj->pos_x)
      return;
  }

  if (potential >= 0 && potential <= 15) {
    s->player_x = potential;
  }
}
