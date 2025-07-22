#include "scn_map.h"
#include "../dude.h"
#include "../ui.h"
#include "../../res/ruins.h"

void scn_map_process(void);
void scn_map_init(void);

const scene_desc scn_map = {
    .process_fn = scn_map_process,
    .init_fn = scn_map_init,
    .finalize_fn = nullptr,
    .id = {'M', 'A', 'P', ' ', ' ', ' '},
};

void scn_map_init(void)
{
    // Load background tile patterns
    set_bkg_data(ruins_TILE_ORIGIN, ruins_TILE_COUNT, ruins_tiles);

    // Transfer color palettes
    set_bkg_palette(0, ruins_PALETTE_COUNT, ruins_palettes);

    // Load background attributes and map
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0, 0, ruins_MAP_ATTRIBUTES_WIDTH, ruins_MAP_ATTRIBUTES_HEIGHT, ruins_map_attributes);
    set_bkg_tile_xy(6, 1, get_bkg_tile_xy(6, 1) | S_PRIORITY); // Make dude appear behind reservoir
    set_bkg_tile_xy(7, 1, get_bkg_tile_xy(6, 1) | S_PRIORITY); // which exist on these 2 positions on the map
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

    // Load and set up UI
    ui_load_gfx();
    ui_draw_panel(2);
    ui_put_text(2, 1, "\"HELLO, WORLD!?\"");
    ui_show_window_top();
}

dude_spr dude = {
    {.frame = 0, .anim = 0, .flipX = false, .x = 112, .y = 32},
    DUDE_WAITING
};

void scn_map_process(void)
{
    dude_update(&dude);
    dude_draw(&dude);

    if (dude.spr.y > 88 && ui_position != UI_TOP)
    {
        ui_show_window_top();
    }
    else if (dude.spr.y < 64 && ui_position != UI_BOTTOM)
    {
        ui_show_window_bottom();
    }
}