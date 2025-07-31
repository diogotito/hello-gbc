#ifndef __MAP_H
#define __MAP_H

#include <stdint.h>
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
    uint8_t *palettes;
} map_gfx;

typedef struct map_desc {
    uint8_t width;
    uint8_t height;
    map_gfx gfx;
    uint8_t *tiles_with_priority;
    uint8_t *tile_passability;
} map_desc;

typedef struct map_state {
    uint8_t unit_count;
    unit_spr units[MAP_MAX_UNITS];
    uint8_t occupancy[18 * 20]; // hardcoded for a map the size of the screen
} map_state;

extern map_desc map_data;
extern map_state map;

void map_init();
void map_load_gfx();

#endif
