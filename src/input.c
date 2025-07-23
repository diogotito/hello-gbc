#include "input.h"

uint8_t cur_joypad;
int8_t cur_joy_dx;
int8_t cur_joy_dy;

bool right_pressed, right_just_pressed;
bool left_pressed,  left_just_pressed;
bool up_pressed,    up_just_pressed;
bool down_pressed,  down_just_pressed;
bool a_pressed,     a_just_pressed;
bool b_pressed,     b_just_pressed;

uint8_t frames_dpad_pressed;

void input_refresh(void) {
    cur_joypad = joypad();
    // cur_joy_dx = (cur_joypad & J_RIGHT) - ((cur_joypad & J_LEFT) >> 1);
    // cur_joy_dy = ((cur_joypad & J_DOWN) >> 3) - ((cur_joypad & J_UP) >> 2);

    bool cur_right = cur_joypad & J_RIGHT;
    bool cur_left = cur_joypad & J_LEFT;
    bool cur_up = cur_joypad & J_UP;
    bool cur_down = cur_joypad & J_DOWN;
    bool cur_a = cur_joypad & J_A;
    bool cur_b = cur_joypad & J_B;

    if (cur_right || cur_left || cur_up || cur_down) {
        frames_dpad_pressed++;
    } else {
        frames_dpad_pressed = 0;
    }

    right_just_pressed = cur_right && !right_pressed;
    left_just_pressed = cur_left && !left_pressed;
    up_just_pressed = cur_up && !up_pressed;
    down_just_pressed = cur_down && !down_pressed;
    a_just_pressed = cur_a && !a_pressed;
    b_just_pressed = cur_b && !b_pressed;

    right_pressed = cur_right;
    left_pressed = cur_left;
    up_pressed = cur_up;
    down_pressed = cur_down;
    a_pressed = cur_a;
    b_pressed = cur_b;
}