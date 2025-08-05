#ifndef __MAP_H
#define __MAP_H

#include <stdint.h>
#include <gbdk/platform.h>
#include "unit.h"

#define MAP_PALETTE_ORIGIN 0
#define MAP_PALETTE_COUNT 5
#define MAP_MAX_UNITS 16
#define MPRI_END 255

typedef struct map_gfx {
    uint16_t tile_imgs_count;
    uint8_t tile_imgs_origin; // base address where tile images are loaded in VRAM
    uint8_t *tile_imgs;
    uint8_t *tile_map;
    uint8_t *tile_attrs;
    uint8_t palettes_count;
    palette_color_t *palettes;
} map_gfx_t;

typedef struct map {
    uint8_t width;
    uint8_t height;
    map_gfx_t gfx;
    uint8_t *tiles_with_priority;
    uint8_t *tile_passability;
} map_desc_t;

#define MAP_DESC(name) map_desc_t name##_map_desc = {                         \
                           .width = name##_MAP_ATTRIBUTES_WIDTH,              \
                           .height = name##_MAP_ATTRIBUTES_HEIGHT,            \
                           .gfx = {                                           \
                               .tile_imgs_count = name##_TILE_COUNT,          \
                               .tile_imgs_origin = name##_TILE_ORIGIN,        \
                               .tile_imgs = name##_tiles,                     \
                               .tile_map = name##_map,                        \
                               .tile_attrs = name##_map_attributes,           \
                               .palettes_count = name##_PALETTE_COUNT,        \
                               .palettes = name##_palettes,                   \
                           },                                                 \
                           .tiles_with_priority = name##_tiles_with_priority, \
                           .tile_passability = name##_tile_passability,       \
}

extern map_desc_t map;

void map_init(map_desc_t*);
void map_load_gfx();

#endif
