/*
Higher level interface to control game rendering
on the LCD hardware.

Uses a singleton for simpler interface.
*/
#ifndef RENDER_H
#define RENDER_H

#include "lcd.h"

// Assumes 16x2 display
const int GRID_HEIGHT = 2;
const int GRID_WIDTH = 16;
typedef char Grid[GRID_HEIGHT][GRID_WIDTH];

/* Bitmap codes */
const int PLAYER_BM_CODE = 0;

void render_init(LCD *lcd);
void render(Grid g);
#endif
