#include "game.h"
#include <Arduino.h>

void GameState_init(GameState *s) {
  memset(s, 0, sizeof(GameState));
  s->player.facing = CENTER;
};

static void run_game_tick(GameState *s, GameInputs *in);

void GameState_update(GameState *s, GameInputs *in) { run_game_tick(s, in); }

static void update_player_facing(GameState *s, GameInputs *in);
static void update_player_pos(GameState *s, GameInputs *in);
static void update_camera_pos(GameState *s);

static void run_game_tick(GameState *s, GameInputs *in) {
  update_player_facing(s, in);
  update_player_pos(s, in);
  update_camera_pos(s);
}

static void update_player_facing(GameState *s, GameInputs *in) {
  if (in->x_move != 0) {
    s->player.facing = (in->x_move == 1) ? RIGHT : LEFT;
  } else if (s->player.on_ground) {
    // only allow face center when standing
    s->player.facing = CENTER;
  }
}

static const int8_t jump_vel[] = {
    [JUMP_NONE] = 0,     //
    [JUMP_START] = 1,    //
    [JUMP_RISING] = 1,   //
    [JUMP_APEX] = 0,     //
    [JUMP_FALLING] = -1, //
};

static inline JumpPhase next_phase(JumpPhase p) {
  return p < JUMP_FALLING ? (JumpPhase)(p + 1) : JUMP_FALLING;
}
static bool is_blocked(GameState *s, int16_t x, int16_t y);

static void update_player_pos(GameState *s, GameInputs *in) {
  // check for jump
  if (in->btn_state == BTN_JUST_PRESSED && s->player.on_ground) {
    s->player.jump_phase = JUMP_START;
  } else if (in->btn_state == BTN_JUST_RELEASED && !s->player.on_ground) {
    s->player.jump_phase =
        s->player.jump_phase < JUMP_APEX ? JUMP_APEX : JUMP_FALLING;
  }

  // apply gravity if just walked off a ledge
  if (!s->player.on_ground && s->player.jump_phase == JUMP_NONE) {
    s->player.jump_phase = JUMP_FALLING;
  }

  int16_t pot_y = s->player.y + jump_vel[s->player.jump_phase];
  int16_t pot_x = s->player.x + in->x_move;

  // resolve y move
  if (!is_blocked(s, s->player.x, pot_y)) {
    s->player.y = pot_y;
  } else {
    s->player.jump_phase = JUMP_FALLING;
  }

  // resolve x move
  if (!is_blocked(s, pot_x, s->player.y)) {
    s->player.x = pot_x;
  }

  // check for ground
  if ((s->player.on_ground = is_blocked(s, s->player.x, s->player.y - 1)))
    s->player.jump_phase = JUMP_NONE;

  // update jump phase
  if (s->player.jump_phase != JUMP_NONE) {
    s->player.jump_phase = next_phase(s->player.jump_phase);
  }
}

static void update_camera_pos(GameState *s) {
  s->camera_x = s->player.x - 8 >= 0 ? s->player.x - 8 : 0;
  s->camera_y = s->player.y > 0 ? s->player.y - 1 : s->player.y;
}

static bool is_blocked(GameState *s, int16_t x, int16_t y) {
  return (y < 0) || s->map[x][y] == TILE_BLOCK;
}
