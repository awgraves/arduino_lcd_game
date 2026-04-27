#include "render.h"
#include <Arduino.h>

// Assumes 16x2 display
static const int GRID_HEIGHT = 2;
static const int GRID_WIDTH = 16;
typedef char Grid[GRID_HEIGHT][GRID_WIDTH];

/* Bitmap codes */
static const int PLAYER_BM_CODE_STOP = 0;
static const int PLAYER_BM_CODE_LEFT = 1;
static const int PLAYER_BM_CODE_RIGHT = 2;

static const Bitmap player_stop = {
    B01110, //
    B01110, //
    B00100, //
    B01110, //
    B10101, //
    B00100, //
    B01010, //
    B01010, //
};

static const Bitmap player_left = {
    B01110, //
    B01110, //
    B00100, //
    B01110, //
    B10101, //
    B00110, //
    B01010, //
    B01001, //
};

static const Bitmap player_right = {
    B01110, //
    B01110, //
    B00100, //
    B01110, //
    B10101, //
    B01100, //
    B01010, //
    B10010, //
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
  LCD_create_char(lcd, PLAYER_BM_CODE_STOP, player_stop);
  LCD_create_char(lcd, PLAYER_BM_CODE_LEFT, player_left);
  LCD_create_char(lcd, PLAYER_BM_CODE_RIGHT, player_right);
}

static char get_player_bm_code(const GameState *s);
static void render_grid(Grid g);

void render(const GameState *s) {
  Grid grid;
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      grid[row][col] = (s->player_x == col) && (s->player_y == row)
                           ? get_player_bm_code(s)
                           : ' ';
    }
  }

  render_grid(grid);
}

/* helpers */
static char get_player_bm_code(const GameState *s) {
  switch (s->player_facing) {
  case LEFT:
    return PLAYER_BM_CODE_LEFT;
  case RIGHT:
    return PLAYER_BM_CODE_RIGHT;
  default:
    return PLAYER_BM_CODE_STOP;
  }
}

static void render_new_at(int row, int col, char c);

static void render_grid(Grid g) {
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      if (r.buf[i][j] != g[i][j]) {
        render_new_at(i, j, g[i][j]);
      }
      r.buf[i][j] = g[i][j];
    }
  }
};

static void render_new_at(int row, int col, char c) {
  LCD_set_cursor(r.lcd, row, col);
  LCD_print(r.lcd, c);
}
