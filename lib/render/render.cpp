#include "render.h"
#include <Arduino.h>

static const Bitmap player_bm = {
    B01110, //
    B01110, //
    B00100, //
    B01110, //
    B11111, //
    B01110, //
    B01010, //
    B01010, //
};

typedef struct {
  LCD *lcd;
  Grid buf;
} Renderer;

static Renderer r;

/* public API */
void render_init(LCD *lcd) {
  r.lcd = lcd;
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      r.buf[i][j] = ' '; // begin with empty spaces in each position
    }
  }
  LCD_create_char(lcd, PLAYER_BM_CODE, player_bm);
}

static void render_new_at(int row, int col, char c);

void render(Grid g) {
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      if (r.buf[i][j] != g[i][j]) {
        render_new_at(i, j, g[i][j]);
      }
      r.buf[i][j] = g[i][j];
    }
  }
};

/* Helpers */
static void render_new_at(int row, int col, char c) {
  LCD_set_cursor(r.lcd, row, col);
  LCD_print(r.lcd, c);
}
