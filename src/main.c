#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/emu_debug.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../res/ruins.h"
#include "input.h"
#include "dude.h"


void init_gfx(void) {
    // Load background tile patterns
    set_bkg_data(ruins_TILE_ORIGIN, ruins_TILE_COUNT, ruins_tiles);

    // Transfer color palettes
    set_bkg_palette(0, ruins_PALETTE_COUNT, ruins_palettes);
    static const palette_color_t black_palette[] = {RGB8(  0,  0,  0), RGB8(159,159,159), RGB8(255, 0, 0), RGB8( 0, 0, 0)};
    set_sprite_palette(1, 1, black_palette);
    
    // Load background attributes and map
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map_attributes);
    VBK_REG = VBK_TILES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map);
    
	// Turn the background map on to make it visible
    SHOW_BKG;

    dude_init_gfx();
    SPRITES_8x8;
    SHOW_SPRITES;
}

void main(void)
{
	init_gfx();
    static dude_spr dude = {
        {.frame = 0, .flipX = false, .x = 112, .y = 32},
        DUDE_WAITING
    };

    while(1) {
        input_refresh();
        dude_update(&dude);
        dude_draw(&dude);
        vsync();
    }
}
