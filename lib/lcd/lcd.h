/*
From scratch replacement
of the LiquidCrystal Arduino lib.

Writing this driver in "C-style" C++
for lower level practice.

Custom tailored to a 16x2 display
(which contains the HD44780)
https://www.alldatasheet.com/datasheet-pdf/view/63673/HITACHI/HD44780.html
*/
#ifndef LCD_H
#define LCD_H
#include <stdint.h>

/*
LCD represents a 16x2 LCD wired for 4-bit mode.
R/W pin should be connected to GND - ie always in 'write' mode.
*/
typedef struct {
  // Register select and enable
  uint8_t RS_PIN;
  uint8_t EN_PIN;
  // 4-bit data pins
  uint8_t D4_PIN;
  uint8_t D5_PIN;
  uint8_t D6_PIN;
  uint8_t D7_PIN;
} LCD;

const uint8_t BITMAP_HEIGHT = 8;
const uint8_t BITMAP_WIDTH = 5;
// 8 rows of 5 bits
typedef uint8_t Bitmap[BITMAP_HEIGHT];

// general commands
void LCD_init(LCD *lcd);
void LCD_clear(LCD *lcd);

// custom character creation
// up to 16 custom chars (zero indexed)
void LCD_create_char(LCD *lcd, uint8_t char_idx, const Bitmap bm);

// controls
void LCD_set_cursor(LCD *lcd, uint8_t row, uint8_t col);
void LCD_print(LCD *lcd, const char *c);
void LCD_print(LCD *lcd, uint8_t n);

#endif
