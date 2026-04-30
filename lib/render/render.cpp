#include "render.h"
#include <Arduino.h>

// Assumes 16x2 display
static const int GRID_HEIGHT = 2;
static const int GRID_WIDTH = 16;
typedef char Grid[GRID_HEIGHT][GRID_WIDTH];

/* Bitmap codes */
typedef enum {
  // LCD is limited to 8 total custom bitmap definitions
  PLAYER_BM_CENTER,
  PLAYER_BM_LEFT_0,
  PLAYER_BM_LEFT_1,
  PLAYER_BM_RIGHT_0,
  PLAYER_BM_RIGHT_1,
  BLOCK_BM,
  FLAG_BM,
} BitmapIDX;

/* Bitmap definitions */
static const Bitmap player_center = {
    B01110, //
    B01110, //
    B00100, //
    B01110, //
    B10101, //
    B00100, //
    B01010, //
    B01010, //
};

static const Bitmap player_left_0 = {
    B01110, //
    B01110, //
    B10100, //
    B11111, //
    B00101, //
    B01100, //
    B01010, //
    B00001, //
};

static const Bitmap player_left_1 = {
    B01110, //
    B01110, //
    B10100, //
    B11111, //
    B00101, //
    B01110, //
    B01010, //
    B01010, //
};

static const Bitmap player_right_0 = {
    B01110, //
    B01110, //
    B00101, //
    B11111, //
    B10100, //
    B00110, //
    B01010, //
    B10000, //
};

static const Bitmap player_right_1 = {
    B01110, //
    B01110, //
    B00101, //
    B11111, //
    B10100, //
    B01110, //
    B01010, //
    B01010, //
};

static const Bitmap block = {
    B11111, //
    B11001, //
    B10101, //
    B10011, //
    B11001, //
    B10101, //
    B10011, //
    B11111, //
};

static const Bitmap flag = {
    B10000, //
    B11000, //
    B11100, //
    B11110, //
    B10000, //
    B10000, //
    B10000, //
    B10000, //
};

typedef struct {
  BitmapIDX frames[2];
  uint8_t frame_idx; // 0 or 1
} Sprite;

Sprite player_left = {.frames = {PLAYER_BM_LEFT_0, PLAYER_BM_LEFT_1},
                      .frame_idx = 0};
Sprite player_right = {.frames = {PLAYER_BM_RIGHT_0, PLAYER_BM_RIGHT_1},
                       .frame_idx = 0};

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
  LCD_create_char(lcd, PLAYER_BM_CENTER, player_center);
  LCD_create_char(lcd, PLAYER_BM_LEFT_0, player_left_0);
  LCD_create_char(lcd, PLAYER_BM_LEFT_1, player_left_1);
  LCD_create_char(lcd, PLAYER_BM_RIGHT_0, player_right_0);
  LCD_create_char(lcd, PLAYER_BM_RIGHT_1, player_right_1);
  LCD_create_char(lcd, BLOCK_BM, block);
  LCD_create_char(lcd, FLAG_BM, flag);
}

static char get_tile_bitmap_code(Tile t);
static char get_player_bitmap_code(const GameState *s);
static void render_grid(Grid g);

void render(const GameState *s) {
  Grid grid;

  // map offsets
  int16_t y_max_vis = s->camera_y + GRID_HEIGHT;
  int16_t x_max_vis = s->camera_x + GRID_WIDTH;

  // place the tiles that would be visible
  for (int y = s->camera_y; y < y_max_vis; y++) {
    for (int x = s->camera_x; x < x_max_vis; x++) {
      // 1 - y inversion because lcd places 0, 0 in TOP left
      // while the game places 0, 0 in BOTTOM left.
      grid[1 - (y - s->camera_y)][x - s->camera_x] =
          get_tile_bitmap_code(s->map[x][y]);
    }
  }

  // add the player
  // same 1 - y inversion as above.
  grid[1 - (s->player.y - s->camera_y)][s->player.x - s->camera_x] =
      get_player_bitmap_code(s);

  render_grid(grid);
}

/* helpers */
static char get_sprite_frame(Sprite *spr) {
  return spr->frames[spr->frame_idx ^= 1];
};

static char get_player_bitmap_code(const GameState *s) {
  if (!s->player.on_ground) {
    return s->player.facing == LEFT ? PLAYER_BM_LEFT_0 : PLAYER_BM_RIGHT_0;
  }
  switch (s->player.facing) {
  case LEFT:
    return get_sprite_frame(&player_left);
  case RIGHT:
    return get_sprite_frame(&player_right);
  default:
    return PLAYER_BM_CENTER;
  }
}

static char get_tile_bitmap_code(Tile t) {
  switch (t) {
  case TILE_EMPTY:
    return ' ';
  case TILE_BLOCK:
    return BLOCK_BM;
  case TILE_FLAG:
    return FLAG_BM;
  case TILE_SPIKE:
    return '*';
  default:
    return ' ';
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
