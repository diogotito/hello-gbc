#ifndef __DUDE_H
#define __DUDE_H

#include <stdint.h>
#include <stdbool.h>
#include "my_sprites.h"

typedef enum DudeState
{
    DUDE_WAITING,
    DUDE_MOVING_RIGHT = J_RIGHT,
    DUDE_MOVING_LEFT = J_LEFT,
    DUDE_MOVING_UP = J_UP,
    DUDE_MOVING_DOWN = J_DOWN,
    DUDE_BLINKING,
} DudeState;

typedef struct dude_spr {
    my_metasprite_t spr;
    DudeState cur_state;
} dude_spr;

void dude_load_gfx();
void dude_update(dude_spr *dude);
void dude_draw(dude_spr *dude);

DudeState dude_handle_movement(dude_spr *dude);

DudeState start_moving_towards(
    dude_spr *dude,
    DudeState next_state, int8_t dx, int8_t dy);

DudeState finish_moving_towards(
    dude_spr *dude,
    DudeState from_state, int8_t dx, int8_t dy);

#endif
