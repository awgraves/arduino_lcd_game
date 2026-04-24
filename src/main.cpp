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

void setup() {
  LCD_init(&lcd);
  LCD_print(&lcd, "Hello world!");
}

void loop() {}
