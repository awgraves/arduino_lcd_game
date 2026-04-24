#include "joystick.h"
#include "lcd.h"
#include <Arduino.h>

LCD lcd = {
    .RS_PIN = 6,
    .EN_PIN = 7,
    .D4_PIN = 10,
    .D5_PIN = 11,
    .D6_PIN = 12,
    .D7_PIN = 13,
};

const Bitmap player_bm = {
    B01110, //
    B01110, //
    B00100, //
    B01110, //
    B11111, //
    B01110, //
    B01010, //
    B01010, //
};

Joystick joy = {.X_PIN = 1, .Y_PIN = 0, .SW_PIN = 3};

void setup() {
  LCD_init(&lcd);
  LCD_print(&lcd, "Hello world!");
  LCD_print(&lcd, B11111111);

  LCD_create_char(&lcd, 0, player_bm);

  LCD_set_cursor(&lcd, 1, 0);
  LCD_print(&lcd, 0);

  Serial.begin(9600);
}

int x_move, y_move;

void loop() {
  x_move = Joystick_X_poll(&joy);
  y_move = Joystick_Y_poll(&joy);

  Serial.print("x: ");
  Serial.println(x_move);

  Serial.print("y: ");
  Serial.println(y_move);

  delay(1000);
}
