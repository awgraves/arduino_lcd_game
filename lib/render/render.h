/*
Higher level interface to control game rendering
on the LCD hardware.

Uses a singleton for simpler interface.
*/
#ifndef RENDER_H
#define RENDER_H

#include "game.h"
#include "lcd.h"

void render_init(LCD *lcd);
void render(const GameState *s);
#endif
