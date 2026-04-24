/*
From scratch replacement
of the LiquidCrystal Arduino lib.

Writing this lib in "C-style" C++
for even lower level practice.

Custom tailored to a 16x2 display
(which contains the HD44780)
https://www.alldatasheet.com/datasheet-pdf/view/63673/HITACHI/HD44780.html
*/
#ifndef LCD_H
#define LCD_H
#include <stdint.h>

// 16x2 in 4-bit mode
typedef struct {
  // Register select and enable
  int RS_PIN;
  int EN_PIN;
  // (R/W pin hard-wired low
  // so always in 'write' mode)
  // 4-bit data pins
  int D4_PIN;
  int D5_PIN;
  int D6_PIN;
  int D7_PIN;
} LCD;

const int GRID_HEIGHT = 2;
const int GRID_WIDTH = 16;
typedef uint8_t Grid[GRID_HEIGHT][GRID_WIDTH];

const int BITMAP_HEIGHT = 8;
const int BITMAP_WIDTH = 5;
// 8 rows of 5 bits
typedef uint8_t Bitmap[BITMAP_HEIGHT];

// general commands
void LCD_init(LCD *lcd);
void LCD_draw(LCD *lcd, Grid grid);
void LCD_clear(LCD *lcd);

// custom character creation
// up to 16 custom chars (idx is zero indexed)
void LCD_create_char(LCD *lcd, int char_idx, const Bitmap bm);

// manual control commands
void LCD_set_cursor(LCD *lcd, int row, int col);
void LCD_print(LCD *lcd, const char *c);
void LCD_print(LCD *lcd, int n);

#endif
