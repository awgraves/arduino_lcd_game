#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H
#include <stdint.h>

typedef enum {
  BTN_OPEN,
  BTN_JUST_PRESSED,
  BTN_HOLD,
  BTN_JUST_RELEASED
} ButtonState;

typedef struct {
  int8_t x_move;
  ButtonState btn_state;
} GameInputs;
#endif
