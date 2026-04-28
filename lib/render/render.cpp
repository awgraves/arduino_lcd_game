#include "render.h"
#include <Arduino.h>

// Assumes 16x2 display
static const int GRID_HEIGHT = 2;
static const int GRID_WIDTH = 16;
typedef char Grid[GRID_HEIGHT][GRID_WIDTH];

/* Bitmap codes */
typedef enum {
  PLAYER_BM_STOP,
  PLAYER_BM_LEFT,
  PLAYER_BM_RIGHT,
  BLOCK_BM,
} BitmapIDX;

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
  LCD_create_char(lcd, PLAYER_BM_STOP, player_stop);
  LCD_create_char(lcd, PLAYER_BM_LEFT, player_left);
  LCD_create_char(lcd, PLAYER_BM_RIGHT, player_right);
  LCD_create_char(lcd, BLOCK_BM, block);
}

static char get_player_bm_code(const GameState *s);
static char get_obj_bm_code(ObjType t);
static void render_grid(Grid g);

void render(const GameState *s) {
  Grid grid;
  // start with blank grid
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      grid[row][col] = ' ';
    }
  }

  // then add objects (1 - y because lcd driver has top row as zero)
  // but game coordinates have ground at 0, and jump is 1
  const Obj *obj;
  int16_t obj_vx;
  int16_t obj_vy;

  for (int i = 0; i < s->object_count; i++) {
    obj = &s->objects[i];
    obj_vx = obj->x - s->camera_x;
    obj_vy = obj->y - s->camera_y;
    // check if object should render in the viewport
    if ((obj_vx > GRID_WIDTH - 1 || obj_vx < 0) ||
        (obj_vy > GRID_HEIGHT - 1 || obj_vy < 0))
      continue;
    grid[1 - obj_vy][obj_vx] = get_obj_bm_code(obj->type);
  }

  // lastly add player
  // same 1 - y inversion as above.
  grid[1 - s->player.y + s->camera_y][s->player.x - s->camera_x] =
      get_player_bm_code(s);

  render_grid(grid);
}

/* helpers */
static char get_player_bm_code(const GameState *s) {
  if (!s->player.on_ground) {
    return '*';
  }
  switch (s->player.facing) {
  case LEFT:
    return PLAYER_BM_LEFT;
  case RIGHT:
    return PLAYER_BM_RIGHT;
  default:
    return PLAYER_BM_STOP;
  }
}

static char get_obj_bm_code(ObjType t) {
  switch (t) {
  case BLOCK:
    return BLOCK_BM;
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
