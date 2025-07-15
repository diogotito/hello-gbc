#include "input.h"

uint8_t cur_joypad;
int8_t cur_joy_dx;
int8_t cur_joy_dy;

void input_refresh(void) {
    cur_joypad = joypad();
    cur_joy_dx = (cur_joypad & J_RIGHT) - ((cur_joypad & J_LEFT) >> 1);
    cur_joy_dy = ((cur_joypad & J_DOWN) >> 3) - ((cur_joypad & J_UP) >> 2);
}