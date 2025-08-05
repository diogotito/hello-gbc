#ifndef __UNIT_H
#define __UNIT_H
/**
 * A unit on the map
 */

#include <stdint.h>
#include <stdbool.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define MAX_UNITS_IN_MAP 8

typedef uint8_t UnitID;
typedef uint8_t UnitType;

typedef enum UnitState {
    UNIT_WAITING,
    UNIT_MOVING_RIGHT = J_RIGHT,
    UNIT_MOVING_LEFT = J_LEFT,
    UNIT_MOVING_UP = J_UP,
    UNIT_MOVING_DOWN = J_DOWN,
    UNIT_BLINKING,
} UnitState;

typedef enum UnitCommands {
    CMD_NOOP = 0,
    CMD_GO_RIGHT = UNIT_MOVING_RIGHT,
    CMD_GO_LEFT = UNIT_MOVING_LEFT,
    CMD_GO_UP = UNIT_MOVING_UP,
    CMD_GO_DOWN = UNIT_MOVING_DOWN,
} UnitCommands;

void unit_init_queue(UnitID);
void unit_enqueue(UnitID, UnitCommands);
UnitCommands unit_dequeue(UnitID);

typedef struct unit_gfx_desc {
    // For loading to VRAM
    uint16_t tile_imgs_count;
    uint8_t tile_imgs_origin;
    uint8_t *tile_imgs;
    uint8_t palettes_count;
    palette_color_t *palettes;

    // For drawing
    uint8_t width;
    uint8_t height;
    metasprite_t **metasprites;
} unit_gfx_desc_t;

#define UNIT_GFX_DESC(name) unit_gfx_desc_t name##_unit_gfx_desc = {    \
                                .tile_imgs_count = name##_TILE_COUNT,   \
                                .tile_imgs_origin = name##_TILE_ORIGIN, \
                                .tile_imgs = name##_tiles,              \
                                .palettes_count = name##_PALETTE_COUNT, \
                                .palettes = name##_palettes,            \
                                .width = name##_WIDTH,                  \
                                .height = name##_HEIGHT,                \
                                .metasprites = name##_metasprites,      \
}

typedef struct unit_spr {
    UnitID id;
    UnitType type;
    UnitState cur_state;
    unit_gfx_desc_t* gfx;
    struct unit_metasprite_state {
        uint8_t frame;
        uint8_t anim;
        uint8_t blinking_countdown;
        bool flipX;
        uint8_t props;
        uint8_t x;
        uint8_t y;
    } spr;
} unit_spr_t;

void unit_load_gfx(UnitType, unit_gfx_desc_t *);
void unit_spr_update(unit_spr_t *);
void unit_spr_draw(unit_spr_t *);

UnitState unit_handle_movement(unit_spr_t *unit);

UnitState start_moving_towards(
    unit_spr_t *unit,
    UnitState next_state, int8_t dx, int8_t dy);

UnitState finish_moving_towards(
    unit_spr_t *unit,
    UnitState from_state, int8_t dx, int8_t dy);

#endif
