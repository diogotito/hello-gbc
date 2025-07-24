#include <stdio.h>
#include "scn_map.h"
#include "../dude.h"
#include "../ui.h"
#include "../../res/ruins.h"

// -----------------
// Scene description
// -----------------

void scn_map_process(void);
void scn_map_init(void);

const scene_desc scn_map = {
    .process_fn = scn_map_process,
    .init_fn = scn_map_init,
    .finalize_fn = nullptr,
    .id = {'M', 'A', 'P', ' ', ' ', ' '},
};

// -----------
// Scene state
// -----------

dude_spr dude = {
    {.frame = 0, .anim = 0, .flipX = false, .x = 112, .y = 32},
    DUDE_WAITING
};

// --------
// Partials
// --------

#include "scn_map__cursor__.c"

// ---------------
// Scene lifecycle
// ---------------

void scn_map_init(void)
{
    // Load background tile patterns
    set_bkg_data(ruins_TILE_ORIGIN, ruins_TILE_COUNT, ruins_tiles);

    // Transfer color palettes
    set_bkg_palette(0, ruins_PALETTE_COUNT, ruins_palettes);

    // Load background attributes and map
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map_attributes);
    set_bkg_tile_xy(6, 1, ruins_map_attributes[26] | S_PRIORITY); // Make dude appear behind reservoir
    set_bkg_tile_xy(7, 1, ruins_map_attributes[27] | S_PRIORITY); // which exist on these 2 positions on the map
    VBK_REG = VBK_TILES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map);

    // Turn the background map on to make it visible
    SHOW_BKG;

    // Load unit sprite
    dude_load_gfx();
    static const palette_color_t black_palette[] = {RGB8(0, 0, 0), RGB8(159, 159, 159), RGB8(255, 0, 0), RGB8(0, 0, 0)};
    set_sprite_palette(1, 1, black_palette);
    SPRITES_8x8;
    SHOW_SPRITES;

    // Load cursor
    cursor_load_gfx();

    // Load and set up UI
    ui_load_gfx();
    ui_draw_panel(2);
    ui_put_text(1, 1, "CUR@( , )");
    ui_put_text(1, 2, "GUY@(   ,   )");
    ui_show_window_top();
}

void scn_map_process(void)
{
    // Dude
    dude_update(&dude);
    dude_draw(&dude);
    
    if (cursor_on_dude && a_just_pressed) {
        dude_enqueue(CMD_GO_UP);
        dude_enqueue(CMD_GO_RIGHT);
        dude_enqueue(CMD_GO_LEFT);
        dude_enqueue(CMD_GO_LEFT);
        dude_enqueue(CMD_GO_DOWN);
        dude_enqueue(CMD_GO_RIGHT);
        dude_enqueue(CMD_GO_DOWN);
    }
    
    if (b_pressed && up_just_pressed)    dude_enqueue(CMD_GO_UP);
    if (b_pressed && down_just_pressed)  dude_enqueue(CMD_GO_DOWN);
    if (b_pressed && left_just_pressed)  dude_enqueue(CMD_GO_LEFT);
    if (b_pressed && right_just_pressed) dude_enqueue(CMD_GO_RIGHT);

    // Cursor
    cursor_update();
    cursor_draw();

    // UI
    if (cursor_y > 5 && ui_position != UI_TOP) {
        ui_show_window_top();
    } else if (cursor_y < 3 && ui_position != UI_BOTTOM) {
        ui_show_window_bottom();
    }
}