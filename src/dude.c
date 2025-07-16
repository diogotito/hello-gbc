#include <gb/gb.h>
#include "../res/dude-sheet.h"
#include "dude.h"
#include "input.h"

static const uint8_t ruins_TILE_PASSABILITY[] = {
    // bits: down | up | left | right
    0,                    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,           0,

    //                [0]     [1]     [2]    [3]      [4]     [5]     [6]     [7]     [8]     [9]
    //          ,---------------------------------------------------------------------------------------.
    0,   /* [0] | */ 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [1] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [2] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [3] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [4] | */ 0b1111, 0b1111, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [5] | */ 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [6] | */ 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1100, 0b1111, /* | */   0,
    0,   /* [7] | */ 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [8] | */ 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    //           `--------------------------------------------------------------------------------------´

    0,                    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,           0,
};

void dude_load_gfx() {
    set_sprite_data(dude_sheet_TILE_ORIGIN, dude_sheet_TILE_COUNT, dude_sheet_tiles);
    set_sprite_palette(0, dude_sheet_PALETTE_COUNT, dude_sheet_palettes);
}

void dude_update(dude_spr *dude)
{
    dude->cur_state = dude_handle_movement(dude);
}

void dude_draw(dude_spr *dude)
{
    if (dude->spr.flipX)
    {
        move_metasprite_flipx(
            dude_sheet_metasprites[(dude->spr.frame & 0x20) >> 5],
            dude_sheet_TILE_ORIGIN, dude->spr.props, /* base tile */ 0,
            dude->spr.x + dude_sheet_WIDTH + 8, dude->spr.y + 16);
    }
    else
    {
        move_metasprite_ex(
            dude_sheet_metasprites[(dude->spr.frame & 0x20) >> 5],
            dude_sheet_TILE_ORIGIN, dude->spr.props, /* base tile */ 0,
            dude->spr.x + 8, dude->spr.y + 16);
    }
}

DudeState dude_handle_movement(dude_spr *dude)
{
    static int8_t dude_speed = 1;

    switch (dude->cur_state)
    {
    case DUDE_WAITING:
        if (cur_joypad & J_RIGHT)
            return start_moving_towards(dude, J_RIGHT, 1, 0);
        if (cur_joypad & J_LEFT)
            return start_moving_towards(dude, J_LEFT, -1, 0);
        if (cur_joypad & J_UP)
            return start_moving_towards(dude, J_UP, 0, -1);
        if (cur_joypad & J_DOWN)
            return start_moving_towards(dude, J_DOWN, 0, 1);
        dude->spr.frame++;
        return DUDE_WAITING;
    case DUDE_MOVING_RIGHT:
        dude->spr.x += dude_speed;
        dude->spr.frame += 4;
        return (dude->spr.x % dude_sheet_WIDTH) ? DUDE_MOVING_RIGHT
                                                : finish_moving_towards(dude, J_RIGHT, 1, 0);
    case DUDE_MOVING_LEFT:
        dude->spr.x -= dude_speed;
        dude->spr.frame += 4;
        return (dude->spr.x % dude_sheet_WIDTH) ? DUDE_MOVING_LEFT
                                                : finish_moving_towards(dude, J_LEFT, -1, 0);
    case DUDE_MOVING_UP:
        dude->spr.y -= dude_speed;
        dude->spr.frame += 4;
        return (dude->spr.y % dude_sheet_HEIGHT) ? DUDE_MOVING_UP
                                                 : finish_moving_towards(dude, J_UP, 0, -1);
    case DUDE_MOVING_DOWN:
        dude->spr.y += dude_speed;
        dude->spr.frame += 4;
        return (dude->spr.y % dude_sheet_HEIGHT) ? DUDE_MOVING_DOWN
                                                 : finish_moving_towards(dude, J_DOWN, 0, 1);
    case DUDE_BLINKING:
        --dude->spr.blinking_countdown;
        uint8_t palette = dude->spr.blinking_countdown % 4 > 1;
        dude->spr.props = dude->spr.props & 0b11111000 | palette;
        return dude->spr.blinking_countdown ? DUDE_BLINKING : DUDE_WAITING;
    }
    return dude->cur_state;
}

DudeState start_moving_towards(
    dude_spr *dude,
    DudeState next_state, int8_t dx, int8_t dy)
{
    // Pick dude flipX based on current and next state
    if (next_state == DUDE_MOVING_LEFT)
    {
        dude->spr.flipX = true;
    }
    else if (next_state == DUDE_MOVING_RIGHT)
    {
        dude->spr.flipX = false;
    }

    // Get dude position in passability map
    uint8_t x = dude->spr.x / 16;
    uint8_t y = dude->spr.y / 16;

    // Check if next tile is passable
    uint8_t check_x = (x + 1) + dx;
    uint8_t check_y = (y + 1) + dy;
    bool canMove = ruins_TILE_PASSABILITY[12 * check_y + check_x] & next_state;

    // Transition state accordingly
    if (canMove)
    {
        return next_state;
    }
    else
    {
        dude->spr.blinking_countdown = 8;
        return DUDE_BLINKING;
    }
}

DudeState finish_moving_towards(
    dude_spr *dude,
    DudeState from_state, int8_t dx, int8_t dy)
{
    // Get dude position in passability map
    uint8_t x = dude->spr.x / 16;
    uint8_t y = dude->spr.y / 16;

    // Check if we could go back to the tile we came from
    uint8_t check_x = (x + 1) - dx;
    uint8_t check_y = (y + 1) - dy;
    bool couldMove = ruins_TILE_PASSABILITY[12 * check_y + check_x] & from_state;

    // If not, then we might have jumped from a cliff or sth. Blink accordingly.
    if (couldMove)
    {
        return DUDE_WAITING;
    }
    else
    {
        dude->spr.blinking_countdown = 4;
        return DUDE_BLINKING;
    }
}
