/**
 * PARTIAL #included from scn_map.c
 * Uses:
 *   unit_spr unit
 */

#include <string.h>
#include "../input.h"
#include "../ui.h"
#include "../../res/cursor.h"

// Cursor stroke animation values
const palette_color_t stroke_anim_values[32] = {
    RGB8(223, 32, 32),
    RGB8(223, 32, 32),
    RGB8(223, 35, 35),
    RGB8(224, 39, 39),
    RGB8(225, 45, 45),
    RGB8(226, 53, 53),
    RGB8(227, 63, 63),
    RGB8(229, 74, 74),
    RGB8(231, 87, 87),
    RGB8(233, 102, 102),
    RGB8(235, 119, 119),
    RGB8(238, 137, 137),
    RGB8(241, 157, 157),
    RGB8(244, 179, 179),
    RGB8(247, 202, 202),
    RGB8(251, 227, 227),
    RGB8(255, 255, 255),
    RGB8(251, 227, 227),
    RGB8(247, 202, 202),
    RGB8(244, 179, 179),
    RGB8(241, 157, 157),
    RGB8(238, 137, 137),
    RGB8(235, 119, 119),
    RGB8(233, 102, 102),
    RGB8(231, 87, 87),
    RGB8(229, 74, 74),
    RGB8(227, 63, 63),
    RGB8(226, 53, 53),
    RGB8(225, 45, 45),
    RGB8(224, 39, 39),
    RGB8(223, 35, 35),
    RGB8(223, 32, 32),
};

static uint8_t cursor_x, cursor_y, cursor_cur_palette;
static bool cursor_on_any_unit;
static bool cursor_on_unit[MAX_UNITS_IN_MAP];

static inline void cursor_load_gfx()
{
    set_sprite_data(cursor_TILE_ORIGIN, cursor_TILE_COUNT, cursor_tiles);
    set_sprite_palette(2, cursor_PALETTE_COUNT, cursor_palettes);

    #define CURSOR_FILL RGB8(255, 151, 146)
    static const palette_color_t cursor_hl_anim_palettes[] = {
        // Palette for cursor highlighting when over a unit
        0, CURSOR_FILL, RGB_CYAN, 0
    };
    set_sprite_palette(3, 1, cursor_hl_anim_palettes);
    set_sprite_palette_entry(3, 2, stroke_anim_values[0]);
}

static inline void cursor_update()
{
    // Move cursor with d-pad
    bool key_repeat = frames_dpad_pressed > 16 && !(frames_dpad_pressed & 0x03);
    if (cursor_x < 9 && (right_just_pressed || right_pressed && key_repeat)) {
        cursor_x += 1;
    }
    if (cursor_x > 0 && (left_just_pressed || left_pressed && key_repeat)) {
        cursor_x -= 1;
    }
    if (cursor_y > 0 && (up_just_pressed || up_pressed && key_repeat)) {
        cursor_y -= 1;
    }
    if (cursor_y < 8 && (down_just_pressed || down_pressed && key_repeat)) {
        cursor_y += 1;
    }

    cursor_on_any_unit = false;
    for (uint8_t i = 0; i < unit_sprites_count; i++) {
        cursor_on_unit[i] = unit_sprites[i].cur_state == UNIT_WAITING &&
            cursor_x == unit_sprites[i].spr.x / 16 &&
            cursor_y == unit_sprites[i].spr.y / 16;
        cursor_on_any_unit = cursor_on_any_unit || cursor_on_unit[i];
    }
    
}

static inline void cursor_draw()
{
    // Update OBJs
    move_metasprite_ex(cursor_metasprites[(sys_time >> 2) % 3], cursor_TILE_ORIGIN,
                       S_PAL(2 + cursor_on_any_unit), 0,
                       cursor_x * 16 + 8, cursor_y * 16 + 16);
    
    // Animate palette
    set_sprite_palette_entry(3, 2, stroke_anim_values[sys_time % 32]);

    // Put some info in the window
    set_win_tile_xy(6, 1, '0' + cursor_x);
    set_win_tile_xy(8, 1, '0' + cursor_y);

    char unit_xy_str[8];
    sprintf(unit_xy_str, "%03d,%03d", unit_sprites[0].spr.x, unit_sprites[0].spr.y);
    ui_put_text(6, 2, "       ");
    set_win_tiles(6, 2, strlen(unit_xy_str), 1, unit_xy_str);

    if (cursor_on_any_unit) {
        ui_put_text(11, 1, "PRESS A!");
    } else {
        ui_put_text(11, 1, "        ");
    }
}
