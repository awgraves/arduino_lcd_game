#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <Arduino.h>

typedef struct {
  uint8_t X_PIN;
  uint8_t Y_PIN;
  uint8_t SW_PIN;

  struct {
    bool prev_sw_read; // raw GPIO read
  } _state;
} Joystick;

typedef enum {
  SW_OPEN,
  SW_JUST_PRESSED,
  SW_HOLD,
  SW_JUST_RELEASED
} SwitchState;

void Joystick_init(Joystick *j);

// poll funcs return -1, 0, 1;
int Joystick_X_poll(Joystick *j);
int Joystick_Y_poll(Joystick *j);

SwitchState Joystick_SW_poll(Joystick *j);
#endif
