#include "game.h"
#include <Arduino.h>

/* Public API */
void GameState_init(GameState *s) {
  memset(s, 0, sizeof(GameState));
  s->player.facing = CENTER;
};

static void update_player_pos(GameState *s, Inputs *in);
static void update_camera_pos(GameState *s);

void GameState_update(GameState *s, Inputs *in) {
  if (in->x_move != 0) {
    s->player.facing = (in->x_move == 1) ? RIGHT : LEFT;
  } else if (s->player.on_ground) {
    // only allow face center when standing
    s->player.facing = CENTER;
  }

  update_player_pos(s, in);
  update_camera_pos(s);
}

/* helpers */
static bool is_blocked(GameState *s, int16_t x, int16_t y);
static bool is_on_ground(GameState *s);

static void update_player_pos(GameState *s, Inputs *in) {
  // gravity
  if (!s->player.on_ground && s->player.y_vel >= 0) {
    // hang time
    if (s->player.y_vel == 0 && s->player.y_hold_remaining) {
      s->player.y_vel = 0;
      s->player.y_hold_remaining--;
    } else {
      s->player.y_vel--;
    }
  }

  // jump
  if (s->player.on_ground && !s->sw_prev_pressed && in->sw_pressed) {
    // long jump (2 tiles)
    s->player.y_vel = 2;
    s->player.y_hold_remaining = 1;
  } else if (!s->player.on_ground && s->sw_prev_pressed && !in->sw_pressed) {
    // cut jump short (1 tile), user let press go early
    s->player.y_vel = 0;
    s->player.y_hold_remaining = 0;
  }
  s->sw_prev_pressed = in->sw_pressed;

  int16_t pot_y = s->player.y + (s->player.y_vel > 0 ? 1 : s->player.y_vel);
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
  if ((s->player.on_ground = is_on_ground(s)))
    s->player.y_vel = 0;
}

static void update_camera_pos(GameState *s) {
  s->camera_x = (s->player.x - 8 >= 0) ? s->player.x - 8 : 0;

  if (s->player.y - s->camera_y > 1) {
    s->camera_y = s->player.y;
  } else if (s->player.y - s->camera_y < 1 && s->player.y > 0) {
    s->camera_y = s->player.y - 1;
  }
}

static bool is_blocked(GameState *s, int16_t x, int16_t y) {
  if (y < 0)
    return true;

  for (int i = 0; i < s->object_count; i++) {
    Obj *obj = &s->objects[i];
    if (obj->type != OBJ_BLOCK)
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
