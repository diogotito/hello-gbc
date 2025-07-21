#include "scn_title.h"
#include "scn_map.h"
#include "../input.h"
#include "../../res/title.h"

void scn_title_process(void);
void scn_title_init(void);

const scene_desc scn_title = {
    .process_fn = scn_title_process,
    .init_fn = scn_title_init,
    .finalize_fn = nullptr,
    .id = {'T', 'I', 'T', 'L', 'E', ' '},
};

void scn_title_init(void)
{
    // Load background tile patterns
    set_bkg_data(title_TILE_ORIGIN, title_TILE_COUNT, title_tiles);

    // Transfer color palettes
    set_bkg_palette(0, title_PALETTE_COUNT, title_palettes);

    // Load background attributes and map
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0, 0, title_MAP_ATTRIBUTES_WIDTH, title_MAP_ATTRIBUTES_HEIGHT, title_map_attributes);
    VBK_REG = VBK_TILES;
    set_bkg_tiles(0, 0, title_MAP_ATTRIBUTES_WIDTH, title_MAP_ATTRIBUTES_HEIGHT, title_map);

    // Turn the background map on to make it visible
    SHOW_BKG;
}

void scn_title_process(void)
{
    if (cur_joypad) {
        scene_call(&scn_map);
    }
}
