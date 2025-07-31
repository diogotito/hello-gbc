#include "../src/game/map.h"
#include "ruins.h"

const uint8_t ruins_tile_passability[] = {
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


const uint8_t ruins_tiles_with_priority[] = {
//  x, y
    6, 1,
    7, 1,
    MPRI_END // end
};

map_desc ruins_map_desc = {
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
    .tiles_with_priority = ruins_tiles_with_priority,
    .tile_passability = ruins_tile_passability,
};