#include "input.h"
#include "input_types.h"

static ButtonState sw_to_btn_state(SwitchState sw) {
  switch (sw) {
  case SW_OPEN:
    return BTN_OPEN;
  case SW_JUST_PRESSED:
    return BTN_JUST_PRESSED;
  case SW_HOLD:
    return BTN_HOLD;
  case SW_JUST_RELEASED:
    return BTN_JUST_RELEASED;
  default:
    return BTN_OPEN;
  }
}

void GameInputs_poll(GameInputs *in, Joystick *joy) {
  in->x_move = Joystick_X_poll(joy);
  in->btn_state = sw_to_btn_state(Joystick_SW_poll(joy));
}
