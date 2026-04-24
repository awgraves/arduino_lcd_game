#include "joystick.h"

void Joystick_init(Joystick *j) {
  pinMode(j->SW_PIN, INPUT);
  pinMode(j->X_PIN, INPUT);
  pinMode(j->Y_PIN, INPUT);
};

int Joystick_X_poll(Joystick *j) {
  // ranges from 5 - 1015
  // right is lower
  int raw = analogRead(j->X_PIN);
  if (raw < 300)
    return 1;
  else if (raw > 720)
    return -1;

  // no movement
  return 0;
};

int Joystick_Y_poll(Joystick *j) {
  // ranges from 5 - 1015
  // up is lower
  int raw = analogRead(j->Y_PIN);
  if (raw < 300)
    return 1;
  else if (raw > 720)
    return -1;

  // no movement
  return 0;
};
