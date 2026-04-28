#include "game.h"
#include "joystick.h"
#include "lcd.h"
#include "render.h"
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
}

void loop() {
  inputs.x_move = Joystick_X_poll(&joy);
  inputs.sw_pressed = Joystick_SW_poll_pressed(&joy);

  GameState_update(&s, &inputs);

  render(&s);
  delay(125);
}
