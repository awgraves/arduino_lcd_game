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

int x_move, y_move;

typedef struct {
  uint8_t row;
  uint8_t col;
} Pos;

Pos pos = {1, 0};

const uint8_t PLAYER_BM_IDX = 0;

Grid grid;

void draw() {
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      grid[row][col] =
          (pos.col == col) && (pos.row == row) ? PLAYER_BM_IDX : ' ';
    }
  }

  LCD_draw(&lcd, grid);
}

void setup() {
  LCD_init(&lcd);
  LCD_create_char(&lcd, PLAYER_BM_IDX, player_bm);
}

void loop() {
  x_move = Joystick_X_poll(&joy);
  y_move = Joystick_Y_poll(&joy);

  int pot_x = x_move + pos.col;
  if (pot_x >= 0 && pot_x <= 15)
    pos.col = pot_x;

  int pot_y = pos.row - y_move;
  if (pot_y >= 0 && pot_y <= 1)
    pos.row = pot_y;

  draw();
}
