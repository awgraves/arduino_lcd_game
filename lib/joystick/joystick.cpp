#include "joystick.h"

void Joystick_init(Joystick *j) {
  // using external 10k pullup resistor
  // for SW pin
  pinMode(j->SW_PIN, INPUT);
  pinMode(j->X_PIN, INPUT);
  pinMode(j->Y_PIN, INPUT);

  j->_state.prev_sw_read = HIGH;
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

SwitchState Joystick_SW_poll(Joystick *j) {
  // LOW means pressed (0)
  // HIGH means not pressed (1)
  bool sw_read = digitalRead(j->SW_PIN);
  bool prev_read = j->_state.prev_sw_read;

  SwitchState result;

  if (sw_read && prev_read) {
    result = SW_OPEN;
  } else if (!sw_read && prev_read) {
    result = SW_JUST_PRESSED;
  } else if (!sw_read && !prev_read) {
    result = SW_HOLD;
  } else {
    result = SW_JUST_RELEASED;
  }

  j->_state.prev_sw_read = sw_read;

  return result;
}
