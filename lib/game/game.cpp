#include "game.h"
#include <Arduino.h>

/* Public API */
void GameState_init(GameState *s) {
  s->player.x = 7;
  s->player.y = 0;
  s->player.facing = CENTER;
  s->player.on_ground = true;
  s->player.y_vel = 0;

  for (int i = 0; i < MAX_OBJECTS; i++)
    s->objects[i] = {.x = 0, .y = 0, .type = NONE};

  // first block
  s->objects[0] = {.x = 0, .y = 0, .type = BLOCK};
  s->objects[1] = {.x = 0, .y = 1, .type = BLOCK};
  s->objects[2] = {.x = 8, .y = 1, .type = BLOCK};
  s->objects[3] = {.x = 12, .y = 0, .type = BLOCK};
  s->objects[4] = {.x = 13, .y = 0, .type = BLOCK};
  s->object_count = 5;
};

static void update_player_pos(GameState *s, Inputs *in);

void GameState_update(GameState *s, Inputs *in) {
  // player bitmap select
  if (in->x_move != 0) {
    s->player.facing = (in->x_move == 1) ? RIGHT : LEFT;
  } else {
    s->player.facing = CENTER;
  }

  update_player_pos(s, in);
}

/* helpers */
static bool is_blocked(GameState *s, int16_t x, int16_t y);
static bool is_on_ground(GameState *s);

static void update_player_pos(GameState *s, Inputs *in) {
  // gravity
  if (!s->player.on_ground)
    s->player.y_vel = s->player.y_vel >= 0 ? s->player.y_vel - 1
                                           : -1; // max -1 block per tick

  // jumps
  if (in->sw_pressed && s->player.on_ground) {
    s->player.y_vel = 1;
  }

  int16_t pot_y = s->player.y + s->player.y_vel;
  int16_t pot_x = s->player.x + in->x_move;

  // resolve y move
  if (!is_blocked(s, s->player.x, pot_y)) {
    s->player.y = pot_y;
  } else {
    // if upward momentum blocked, reset it
    if (s->player.y_vel > 0)
      s->player.y_vel = 0;
  }

  // resolve x move
  if (!is_blocked(s, pot_x, s->player.y)) {
    s->player.x = pot_x;
  }

  // check for ground
  s->player.on_ground = is_on_ground(s);
}

static bool is_blocked(GameState *s, int16_t x, int16_t y) {
  if (y < 0)
    return true;

  for (int i = 0; i < s->object_count; i++) {
    Obj *obj = &s->objects[i];
    if (obj->type != BLOCK)
      continue;

    if (obj->x == x && obj->y == y)
      return true;
  }

  return false;
}

static bool is_on_ground(GameState *s) {
  if (s->player.y == 0)
    return true;
  return is_blocked(s, s->player.x, s->player.y - 1);
}
