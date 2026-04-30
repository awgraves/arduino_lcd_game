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

static void update_player_pos(GameState *s, Inputs *in) {
  // gravity
  if (!s->player.on_ground && s->player.y_vel >= 0) {
    // hang time at the top
    if (s->player.y_vel == 0 && s->player.y_hold_remaining) {
      s->player.y_vel = 0;
      s->player.y_hold_remaining--;
    } else {
      s->player.y_vel--;
    }
  }

  // jump
  bool sw_just_pressed = in->sw_pressed && !s->sw_prev_pressed;
  bool sw_just_released = !in->sw_pressed && s->sw_prev_pressed;
  if (sw_just_pressed && s->player.on_ground) {
    // long jump (2 tiles)
    s->player.y_vel = 2;
    s->player.y_hold_remaining = 1;
  } else if (sw_just_released && !s->player.on_ground) {
    // cut jump short if user let go early
    s->player.y_vel = s->player.y_hold_remaining = 0;
  }
  s->sw_prev_pressed = in->sw_pressed;

  // calc next potential axis destinations
  int16_t pot_y = s->player.y + (s->player.y_vel > 0 ? 1 : s->player.y_vel);
  int16_t pot_x = s->player.x + in->x_move;

  // resolve y move
  if (!is_blocked(s, s->player.x, pot_y)) {
    s->player.y = pot_y;
  } else if (s->player.y_vel > 0) {
    s->player.y_vel = s->player.y_hold_remaining = 0;
  }

  // resolve x move
  if (!is_blocked(s, pot_x, s->player.y)) {
    s->player.x = pot_x;
  }

  // check for ground
  if ((s->player.on_ground = is_blocked(s, s->player.x, s->player.y - 1)))
    s->player.y_vel = 0;
}

static void update_camera_pos(GameState *s) {
  s->camera_x = s->player.x - 8 >= 0 ? s->player.x - 8 : 0;
  s->camera_y = s->player.y > 0 ? s->player.y - 1 : s->player.y;
}

static bool is_blocked(GameState *s, int16_t x, int16_t y) {
  return (y < 0) || s->map[x][y] == TILE_BLOCK;
}
