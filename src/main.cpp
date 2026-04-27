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

int x_move, y_move;

typedef struct {
  uint8_t row;
  uint8_t col;
} Pos;

Pos pos = {1, 0};

Grid grid;

void setup() {
  LCD_init(&lcd);
  render_init(&lcd);
}

void update_grid() {
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      grid[row][col] =
          (pos.col == col) && (pos.row == row) ? PLAYER_BM_CODE : ' ';
    }
  }

  render(grid);
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

  update_grid();
  delay(150);
}
