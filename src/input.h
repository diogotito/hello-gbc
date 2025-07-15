#ifndef __INPUT_H
#define __INPUT_H

#include <stdint.h>
#include <gb/gb.h>

extern uint8_t cur_joypad;
extern int8_t cur_joy_dx;
extern int8_t cur_joy_dy;

void input_refresh(void);

#endif