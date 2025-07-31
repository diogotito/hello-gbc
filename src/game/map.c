#include "map.h"
#include "../../res/ruins.h"

#include "map__ruins__.c"

map_desc map_data = {
    .width = ruins_MAP_ATTRIBUTES_WIDTH,
    .height = ruins_MAP_ATTRIBUTES_HEIGHT,
    .gfx = {
        .tile_imgs_count = ruins_TILE_COUNT,
        .tile_imgs_origin = ruins_TILE_ORIGIN,
        .tile_imgs = ruins_tiles,
        .tile_map = ruins_map,
        .tile_attrs = ruins_map_attributes,
        .palettes_count = ruins_PALETTE_COUNT,
        .palettes = ruins_palettes,
    },
    .tiles_with_priority = ruins_TILES_WITH_PRIORITY, // map__ruins__.c
    .tile_passability = ruins_TILE_PASSABILITY, // map__ruins__.c
};

map_state map = {};

void map_init() {
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