#include "game.h"
#include "input.h"
#include "input_types.h"
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

Joystick joystick = {.X_PIN = 1, .Y_PIN = 0, .SW_PIN = 3};

GameInputs inputs;
GameState s;

void reset_game() {
  GameState_init(&s);
  load_world(&s);
  render_init(&lcd);
}

void setup() {
  LCD_init(&lcd);
  reset_game();
}

void loop() {
  GameInputs_poll(&inputs, &joystick);
  GameState_update(&s, &inputs);
  render(&s);

  delay(125);
}
