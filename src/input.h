#ifndef __INPUT_H
#define __INPUT_H

#include <gb/gb.h>
#include <stdint.h>

extern uint8_t cur_joypad;
extern int8_t cur_joy_dx;
extern int8_t cur_joy_dy;

void input_refresh(void);

#endif