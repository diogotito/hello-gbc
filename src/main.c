#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/incbin.h>
#include <stdint.h>
#include "../res/ruins.h"
#include "../res/dude-sheet.h"


void init_gfx(void) {
    // Load background tile patterns
    set_bkg_data(ruins_TILE_ORIGIN, ruins_TILE_COUNT, ruins_tiles);
    set_sprite_data(dude_sheet_TILE_ORIGIN, dude_sheet_TILE_COUNT, dude_sheet_tiles);

    // Transfer color palettes
    set_bkg_palette(0, ruins_PALETTE_COUNT, ruins_palettes);
    set_sprite_palette(0, dude_sheet_PALETTE_COUNT, dude_sheet_palettes);
    
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
    int frame;
    int x;
    int y;
} dude = {0, 32, 32};

void main(void)
{
	init_gfx();

    while(1) {
        // Input processing
        cur_joypad = joypad();
        cur_joy_dx = ( cur_joypad & J_RIGHT)      - ((cur_joypad & J_LEFT) >> 1);
        cur_joy_dy = ((cur_joypad & J_DOWN) >> 3) - ((cur_joypad & J_UP)   >> 2);

        // Dude processing
        dude.frame += (sys_time % 20) == 0;  // Advance animation every 20 V-blanks
        dude.frame %= 2;                     // There are 2 frames in the animation
        dude.x += cur_joy_dx;
        dude.y += cur_joy_dy;

        // Update dude OBJs in shadow OAM
        move_metasprite_ex(
            dude_sheet_metasprites[dude.frame],
            dude_sheet_TILE_ORIGIN, 0x00, 0, dude.x, dude.y);

		// Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}
