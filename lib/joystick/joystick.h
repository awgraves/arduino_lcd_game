#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <Arduino.h>

typedef struct {
  int X_PIN;
  int Y_PIN;
  int SW_PIN;
} Joystick;

void Joystick_init(Joystick *j);

// poll funcs return -1, 0, 1;
int Joystick_X_poll(Joystick *j);
int Joystick_Y_poll(Joystick *j);

#endif
