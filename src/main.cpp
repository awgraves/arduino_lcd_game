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

void setup() {
  LCD_init(&lcd);
  LCD_print(&lcd, "Hello world!");
  LCD_print(&lcd, B11111111);

  LCD_create_char(&lcd, 0, player_bm);

  LCD_set_cursor(&lcd, 1, 0);
  LCD_print(&lcd, 0);
}

void loop() {}
