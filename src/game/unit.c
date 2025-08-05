#include <gb/gb.h>
#include "unit.h"
#include "../input.h"
#include "map.h"

#define MAX_COMMANDS 32

// -------------
// Command queue
// -------------

struct {
    UnitCommands queue[MAX_COMMANDS];
    uint8_t back;
    uint8_t front;
} unit_cmds[MAX_UNITS_IN_MAP];

void unit_init_queue(UnitID id) {
    unit_cmds[id].back = unit_cmds[id].front = 0;
    unit_cmds[id].queue[0] = CMD_NOOP;
}

void unit_enqueue(UnitID id, UnitCommands cmd) {
    if ((unit_cmds[id].back + 1) % MAX_COMMANDS == unit_cmds[id].front) {
        return;
    }
    unit_cmds[id].queue[unit_cmds[id].back] = cmd;
    unit_cmds[id].back = (unit_cmds[id].back + 1) % MAX_COMMANDS;
}

UnitCommands unit_dequeue(UnitID id) {
    if (unit_cmds[id].front == unit_cmds[id].back) {
        return CMD_NOOP;
    }
    UnitCommands cmd = unit_cmds[id].queue[unit_cmds[id].front];
    unit_cmds[id].front = (unit_cmds[id].front + 1) % MAX_COMMANDS;
    return cmd;
}

// ---------
// Lifecycle
// ---------

void unit_load_gfx(UnitType palette_origin, unit_gfx_desc_t *gfx)
{
    set_sprite_data(gfx->tile_imgs_origin, gfx->tile_imgs_count, gfx->tile_imgs);
    set_sprite_palette(palette_origin, gfx->palettes_count, gfx->palettes);
}

void unit_spr_update(unit_spr_t *unit)
{
    unit->cur_state = unit_handle_movement(unit);
}

void unit_spr_draw(unit_spr_t *unit)
{
    if (unit->spr.flipX)
    {
        move_metasprite_flipx(
            unit->gfx->metasprites[unit->spr.anim + ((unit->spr.frame & 0x20) >> 5)],
            unit->gfx->tile_imgs_origin, unit->spr.props, 8 + 4 * unit->id,
            unit->spr.x + unit->gfx->width + 8, unit->spr.y + 16);
    }
    else
    {
        move_metasprite_ex(
            unit->gfx->metasprites[unit->spr.anim + ((unit->spr.frame & 0x20) >> 5)],
            unit->gfx->tile_imgs_origin, unit->spr.props, 8 + 4 * unit->id,
            unit->spr.x + 8, unit->spr.y + 16);
    }
}

UnitState unit_process_next_command(unit_spr_t *unit)
{
    switch (unit_dequeue(unit->id))
    {
    case CMD_GO_RIGHT:
        return start_moving_towards(unit, UNIT_MOVING_RIGHT, 1, 0);
    case CMD_GO_LEFT:
        return start_moving_towards(unit, UNIT_MOVING_LEFT, -1, 0);
    case CMD_GO_UP:
        return start_moving_towards(unit, UNIT_MOVING_UP, 0, -1);
    case CMD_GO_DOWN:
        return start_moving_towards(unit, UNIT_MOVING_DOWN, 0, 1);
    default:
        return UNIT_WAITING;
    }
}

UnitState unit_handle_movement(unit_spr_t *unit)
{
    static int8_t unit_speed = 1;

    switch (unit->cur_state)
    {
    case UNIT_WAITING:
        unit->spr.frame++;
        return unit_process_next_command(unit);
    case UNIT_MOVING_RIGHT:
        unit->spr.x += unit_speed;
        unit->spr.frame += 4;
        return (unit->spr.x % unit->gfx->width) ? UNIT_MOVING_RIGHT
                                                : finish_moving_towards(unit, UNIT_MOVING_RIGHT, 1, 0);
    case UNIT_MOVING_LEFT:
        unit->spr.x -= unit_speed;
        unit->spr.frame += 4;
        return (unit->spr.x % unit->gfx->width) ? UNIT_MOVING_LEFT
                                                : finish_moving_towards(unit, UNIT_MOVING_LEFT, -1, 0);
    case UNIT_MOVING_UP:
        unit->spr.y -= unit_speed;
        unit->spr.frame += 4;
        return (unit->spr.y % unit->gfx->height) ? UNIT_MOVING_UP
                                                 : finish_moving_towards(unit, UNIT_MOVING_UP, 0, -1);
    case UNIT_MOVING_DOWN:
        unit->spr.y += unit_speed;
        unit->spr.frame += 4;
        return (unit->spr.y % unit->gfx->height) ? UNIT_MOVING_DOWN
                                                 : finish_moving_towards(unit, UNIT_MOVING_DOWN, 0, 1);
    case UNIT_BLINKING:
        --unit->spr.blinking_countdown;
        uint8_t palette = unit->spr.blinking_countdown % 4 > 1;
        unit->spr.props = unit->spr.props & 0b11111000 | S_PAL(palette);
        return unit->spr.blinking_countdown ? UNIT_BLINKING : UNIT_WAITING;
    }
    return unit->cur_state;
}

UnitState start_moving_towards(
    unit_spr_t *unit,
    UnitState next_state, int8_t dx, int8_t dy)
{
    // Pick unit flipX based on current and next state
    if (next_state == UNIT_MOVING_LEFT)
    {
        unit->spr.flipX = true;
    }
    else if (next_state == UNIT_MOVING_RIGHT)
    {
        unit->spr.flipX = false;
    }
    else if (next_state == UNIT_MOVING_UP)
    {
        unit->spr.anim = 2;
    }
    else if (next_state == UNIT_MOVING_DOWN)
    {
        unit->spr.anim = 0;
    }

    // Get unit position in passability map
    uint8_t x = unit->spr.x / 16;
    uint8_t y = unit->spr.y / 16;

    // Check if next tile is passable
    uint8_t check_x = (x + 1) + dx;
    uint8_t check_y = (y + 1) + dy;
    bool canMove = map.tile_passability[12 * check_y + check_x] & next_state;

    // Transition state accordingly
    if (canMove)
    {
        return next_state;
    }
    else
    {
        unit->spr.blinking_countdown = 8;
        return UNIT_BLINKING;
    }
}

UnitState finish_moving_towards(
    unit_spr_t *unit,
    UnitState from_state, int8_t dx, int8_t dy)
{
    // Get unit position in passability map
    uint8_t x = unit->spr.x / 16;
    uint8_t y = unit->spr.y / 16;

    // Check if we could go back to the tile we came from
    uint8_t check_x = (x + 1) - dx;
    uint8_t check_y = (y + 1) - dy;
    bool couldMove = map.tile_passability[12 * check_y + check_x] & from_state;

    // If not, then we might have jumped from a cliff or sth. Blink accordingly.
    if (couldMove)
    {
        return UNIT_WAITING;
    }
    else
    {
        unit->spr.blinking_countdown = 4;
        return UNIT_BLINKING;
    }
}
