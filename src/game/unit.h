#ifndef __UNIT_H
#define __UNIT_H
/**
 * A unit on the map
 */

#include <stdint.h>
#include <stdbool.h>
#include <gb/gb.h>
#include "../my_sprites.h"

typedef enum UnitState {
    UNIT_WAITING,
    UNIT_MOVING_RIGHT = J_RIGHT,
    UNIT_MOVING_LEFT = J_LEFT,
    UNIT_MOVING_UP = J_UP,
    UNIT_MOVING_DOWN = J_DOWN,
    UNIT_BLINKING,
} UnitState;

typedef enum UnitCommands {
    CMD_NOOP = 0,
    CMD_GO_RIGHT = UNIT_MOVING_RIGHT,
    CMD_GO_LEFT = UNIT_MOVING_LEFT,
    CMD_GO_UP = UNIT_MOVING_UP,
    CMD_GO_DOWN = UNIT_MOVING_DOWN,
} UnitCommands;

void unit_enqueue(UnitCommands cmd);
UnitCommands unit_dequeue();

typedef struct unit_spr {
    my_metasprite_t spr;
    UnitState cur_state;
} unit_spr;

void unit_load_gfx();
void unit_update(unit_spr *unit);
void unit_draw(unit_spr *unit);

UnitState unit_handle_movement(unit_spr *unit);

UnitState start_moving_towards(
    unit_spr *unit,
    UnitState next_state, int8_t dx, int8_t dy);

UnitState finish_moving_towards(
    unit_spr *unit,
    UnitState from_state, int8_t dx, int8_t dy);

#endif
