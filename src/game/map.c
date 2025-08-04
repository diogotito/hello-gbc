#include "map.h"
#include <gbdk/platform.h>

map_desc_t map;

void map_init(map_desc_t *map_data_to_load) {
    map = *map_data_to_load;
}

void map_load_gfx() {
    // Load background tile patterns
    set_bkg_data(
        map.gfx.tile_imgs_origin,
        map.gfx.tile_imgs_count,
        map.gfx.tile_imgs
    );

    // Transfer color palettes
    set_bkg_palette(
        MAP_PALETTE_ORIGIN,
        map.gfx.palettes_count,
        map.gfx.palettes
    );

    // Load background attributes
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(
        0, 0,
        map.width, map.height,
        map.gfx.tile_attrs
    );

    // Set priority bits in attributes
    // TODO think of a way of setting tile priorities through a tool like
    // Aseprite or Tiled or YAML files + some Python script for codegen.
    for (uint8_t i = 0; map.tiles_with_priority[i] != MPRI_END; i += 2) {
        uint8_t x = map.tiles_with_priority[i];
        uint8_t y = map.tiles_with_priority[i + 1];
        uint8_t tile_i = map.width * y + x;
        set_bkg_tile_xy(x, y, map.gfx.tile_attrs[tile_i] | S_PRIORITY);
    }
    
    // Load background map
    VBK_REG = VBK_TILES;
    set_bkg_tiles(
        0, 0,
        map.width, map.height,
        map.gfx.tile_map
    );
}