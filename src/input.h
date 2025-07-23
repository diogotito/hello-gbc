#ifndef __INPUT_H
#define __INPUT_H

#include <gb/gb.h>
#include <stdint.h>

extern uint8_t cur_joypad;
extern int8_t cur_joy_dx;
extern int8_t cur_joy_dy;

extern bool right_pressed, right_just_pressed;
extern bool left_pressed,  left_just_pressed;
extern bool up_pressed,    up_just_pressed;
extern bool down_pressed,  down_just_pressed;
extern bool a_pressed,  a_just_pressed;
extern bool b_pressed,  b_just_pressed;

extern uint8_t frames_dpad_pressed;

void input_refresh(void);

#endif