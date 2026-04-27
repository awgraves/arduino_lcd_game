#include "game.h"
#include <Arduino.h>

/* Public API */
void GameState_init(GameState *s) {
  s->player_x = 7;
  s->player_y = 1;
  s->player_facing = STOP;
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

  if (potential >= 0 && potential <= 15) {
    s->player_x = potential;
  }
}
