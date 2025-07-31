#include "map.h"
#include "../../res/ruins.h"

#include "map__ruins__.c"

map_desc map_data;

map_state map = {
    .unit_count = 0
};

void map_init() {
    map_data = map_data__ruins; // TODO get this from a pointer passed to a "map_load()" maybe
    map.unit_count = 0;
}

void map_load_gfx() {
    // Load background tile patterns
    set_bkg_data(
        map_data.gfx.tile_imgs_origin,
        map_data.gfx.tile_imgs_count,
        map_data.gfx.tile_imgs
    );

    // Transfer color palettes
    set_bkg_palette(
        MAP_PALETTE_ORIGIN,
        map_data.gfx.palettes_count,
        map_data.gfx.palettes
    );

    // Load background attributes
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(
        0, 0,
        map_data.width, map_data.height,
        map_data.gfx.tile_attrs
    );

    // Set priority bits in attributes
    for (uint8_t i = 0; map_data.tiles_with_priority[i] != MPRI_END; i += 2) {
        uint8_t x = map_data.tiles_with_priority[i];
        uint8_t y = map_data.tiles_with_priority[i + 1];
        uint8_t tile_i = map_data.width * y + x;
        set_bkg_tile_xy(x, y, map_data.gfx.tile_attrs[tile_i] | S_PRIORITY);
    }
    
    // Load background map
    VBK_REG = VBK_TILES;
    set_bkg_tiles(
        0, 0,
        map_data.width, map_data.height,
        map_data.gfx.tile_map
    );
}