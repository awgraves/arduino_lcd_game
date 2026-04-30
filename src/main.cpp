#include "game.h"
#include "joystick.h"
#include "lcd.h"
#include "render.h"
#include "world.h"
#include <Arduino.h>

LCD lcd = {
    .RS_PIN = 6,
    .EN_PIN = 7,
    .D4_PIN = 10,
    .D5_PIN = 11,
    .D6_PIN = 12,
    .D7_PIN = 13,
};

Joystick joy = {.X_PIN = 1, .Y_PIN = 0, .SW_PIN = 3};

Inputs inputs;
GameState s;

void setup() {
  LCD_init(&lcd);
  render_init(&lcd);

  GameState_init(&s);
  load_world(&s);
}

ButtonState sw_to_btn_state(SwitchState sw) {
  switch (sw) {
  case SW_OPEN:
    return BTN_OPEN;
  case SW_JUST_PRESSED:
    return BTN_JUST_PRESSED;
  case SW_HOLD:
    return BTN_HOLD;
  case SW_JUST_RELEASED:
    return BTN_JUST_RELEASED;
  }
}

inline void poll_inputs(Inputs *in) {
  in->x_move = Joystick_X_poll(&joy);
  in->btn_state = sw_to_btn_state(Joystick_SW_poll(&joy));
}

void loop() {
  poll_inputs(&inputs);
  GameState_update(&s, &inputs);
  render(&s);

  delay(125);
}
