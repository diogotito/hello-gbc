#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/incbin.h>
#include <gbdk/emu_debug.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../res/ruins.h"
#include "../res/dude-sheet.h"

const uint8_t ruins_TILE_PASSABILITY[] = {
    // bits: down | up | left | right
    0,                    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,           0,

    //                [0]     [1]     [2]    [3]      [4]     [5]     [6]     [7]     [8]     [9]
    //          ,---------------------------------------------------------------------------------------.
    0,   /* [0] | */ 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [1] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [2] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [3] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [4] | */ 0b1111, 0b1111, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [5] | */ 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [6] | */ 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1100, 0b1111, /* | */   0,
    0,   /* [7] | */ 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    0,   /* [8] | */ 0b0000, 0b0000, 0b0000, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, /* | */   0,
    //           `--------------------------------------------------------------------------------------´

    0,                    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,           0,
};


void init_gfx(void) {
    // Load background tile patterns
    set_bkg_data(ruins_TILE_ORIGIN, ruins_TILE_COUNT, ruins_tiles);
    set_sprite_data(dude_sheet_TILE_ORIGIN, dude_sheet_TILE_COUNT, dude_sheet_tiles);

    // Transfer color palettes
    set_bkg_palette(0, ruins_PALETTE_COUNT, ruins_palettes);
    set_sprite_palette(0, dude_sheet_PALETTE_COUNT, dude_sheet_palettes);
    static const palette_color_t black_palette[] = {RGB8(  230,  230,  230), RGB8(80,80,80), RGB8(13, 16, 8), RGB8( 16, 8, 8)};
    set_sprite_palette(1, 1, black_palette);
    
    // Load background attributes and map
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map_attributes);
    VBK_REG = VBK_TILES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map);
    
	// Turn the background map on to make it visible
    SHOW_BKG;

    // Show the dude
    // set_sprite_tile(0, dude_sheet_TILE_ORIGIN);
    // set_sprite_tile(1, dude_sheet_TILE_ORIGIN + 2);
    move_metasprite_ex(dude_sheet_metasprites[0], dude_sheet_TILE_ORIGIN, 0x00, 0, 32, 32);
    SPRITES_8x16;
    SHOW_SPRITES;
}


// State
uint8_t cur_joypad;
int8_t cur_joy_dx, cur_joy_dy;

struct my_metasprite {
    uint8_t frame;
    uint8_t blinking_countdown;
    bool flipX;
    uint8_t props;
    uint8_t x;
    uint8_t y;
};

enum DudeState
{
    DUDE_WAITING,
    DUDE_MOVING_RIGHT = J_RIGHT,
    DUDE_MOVING_LEFT  = J_LEFT,
    DUDE_MOVING_UP    = J_UP,
    DUDE_MOVING_DOWN  = J_DOWN,
    DUDE_BLINKING,
} cur_state = DUDE_WAITING;

enum DudeState start_moving_towards(
    struct my_metasprite *dude,
    enum DudeState next_state, int8_t dx, int8_t dy)
{
    // Pick dude flipX based on current and next state
    if (next_state == DUDE_MOVING_LEFT) {
        dude->flipX = true;
    } else if (next_state == DUDE_MOVING_RIGHT) {
        dude->flipX = false;
    }
    
    // Get dude position in passability map
    uint8_t x = dude->x / 16;
    uint8_t y = dude->y / 16;

    // Check if next tile is passable
    uint8_t check_x = (x + 1) + dx;
    uint8_t check_y = (y + 1) + dy;
    bool canMove = ruins_TILE_PASSABILITY[12 * check_y + check_x] & next_state;

    // Transition state accordingly
    if (canMove) {
        return next_state;
    } else {
        dude->blinking_countdown = 18;
        return DUDE_BLINKING;
    }
}


enum DudeState dude_handle_movement(struct my_metasprite *dude)
{
    static int8_t dude_speed = 1;

    switch (cur_state)
    {
    case DUDE_WAITING:
        if (cur_joypad & J_RIGHT) return start_moving_towards(dude, J_RIGHT,  1,  0);
        if (cur_joypad & J_LEFT)  return start_moving_towards(dude, J_LEFT,  -1,  0);
        if (cur_joypad & J_UP)    return start_moving_towards(dude, J_UP,     0, -1);
        if (cur_joypad & J_DOWN)  return start_moving_towards(dude, J_DOWN,   0,  1);
        dude->frame++;
        return DUDE_WAITING;
    case DUDE_MOVING_RIGHT:
        dude->x += dude_speed;
        dude->frame += 4;
        return (dude->x % dude_sheet_WIDTH) ? DUDE_MOVING_RIGHT : DUDE_WAITING;
    case DUDE_MOVING_LEFT:
        dude->x -= dude_speed;
        dude->frame += 4;
        return (dude->x % dude_sheet_WIDTH) ? DUDE_MOVING_LEFT : DUDE_WAITING;
    case DUDE_MOVING_UP:
        dude->y -= dude_speed;
        dude->frame += 4;
        return (dude->y % dude_sheet_HEIGHT) ? DUDE_MOVING_UP : DUDE_WAITING;
    case DUDE_MOVING_DOWN:
        dude->y += dude_speed;
        dude->frame += 4;
        return (dude->y % dude_sheet_HEIGHT) ? DUDE_MOVING_DOWN : DUDE_WAITING;
    case DUDE_BLINKING:
        --dude->blinking_countdown;
        uint8_t palette = dude->blinking_countdown % 6 > 2;
        dude->props = dude->props & 0b11111000 | palette;
        return dude->blinking_countdown ? DUDE_BLINKING : DUDE_WAITING;
    }
    return cur_state;
}

void main(void)
{
	init_gfx();
    static struct my_metasprite dude = { .frame = 0, .flipX = false, .x = 112, .y = 32 };

    while(1) {
        // Input processing
        cur_joypad = joypad();
        cur_joy_dx = ( cur_joypad & J_RIGHT)      - ((cur_joypad & J_LEFT) >> 1);
        cur_joy_dy = ((cur_joypad & J_DOWN) >> 3) - ((cur_joypad & J_UP)   >> 2);

        // Dude processing
        cur_state = dude_handle_movement(&dude);

        // Update dude OBJs in shadow OAM
        
        if (dude.flipX)
        {
            move_metasprite_flipx(
                dude_sheet_metasprites[(dude.frame & 0x20) >> 5],
                dude_sheet_TILE_ORIGIN, dude.props, 0, dude.x + dude_sheet_WIDTH + 8, dude.y + 16);
        } else {
            move_metasprite_ex(
                dude_sheet_metasprites[(dude.frame & 0x20) >> 5],
                dude_sheet_TILE_ORIGIN, dude.props, 0, dude.x + 8, dude.y + 16);
        }

        // Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}
