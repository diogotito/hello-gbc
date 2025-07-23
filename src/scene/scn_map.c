#include <stdio.h>
#include "scn_map.h"
#include "../dude.h"
#include "../ui.h"
#include "../input.h"
#include "../../res/ruins.h"
#include "../../res/cursor.h"

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

    // Load cursor
    set_sprite_data(cursor_TILE_ORIGIN, cursor_TILE_COUNT, cursor_tiles);
    set_sprite_palette(2, cursor_PALETTE_COUNT, cursor_palettes);
    static const palette_color_t white_palette[] = {RGB8(0, 0, 0), RGB8(255, 255, 255), RGB8(255, 255, 255), RGB8(255, 255, 255)};
    set_sprite_palette(3, 1, white_palette);

    // Load and set up UI
    ui_load_gfx();
    ui_draw_panel(2);
    ui_put_text(1, 1, "CUR@( , )");
    ui_put_text(1, 2, "GUY@(   ,   )");
    ui_show_window_top();
}

dude_spr dude = {
    {.frame = 0, .anim = 0, .flipX = false, .x = 112, .y = 32},
    DUDE_WAITING
};

static uint8_t cursor_x, cursor_y, cursor_cur_palette;
static bool cursor_on_dude;

static inline void cursor_update()
{
    // Update based on D-pad input
    if (cursor_x < 9 && (right_just_pressed || right_pressed && frames_dpad_pressed > 16 && !(frames_dpad_pressed & 0x03)))  cursor_x += 1;
    if (cursor_x > 0 && (left_just_pressed || left_pressed && frames_dpad_pressed > 16 && !(frames_dpad_pressed & 0x03)))    cursor_x -= 1;
    if (cursor_y > 0 && (up_just_pressed || up_pressed && frames_dpad_pressed > 16 && !(frames_dpad_pressed & 0x03)))        cursor_y -= 1;
    if (cursor_y < 8 && (down_just_pressed || down_pressed && frames_dpad_pressed > 16 && !(frames_dpad_pressed & 0x03)))    cursor_y += 1;

    cursor_on_dude = (cursor_x == dude.spr.x / 16 && cursor_y == dude.spr.y / 16);
}

static inline void cursor_draw()
{
    move_metasprite_ex(cursor_metasprites[(sys_time >> 2) % 3], cursor_TILE_ORIGIN,
                       2 + cursor_on_dude, 0,
                       cursor_x * 16 + 8, cursor_y * 16 + 16);
}

void scn_map_process(void)
{
    // Dude
    dude_update(&dude);
    dude_draw(&dude);
    
    if (cursor_on_dude && a_just_pressed) {
        dude_enqueue(CMD_GO_UP);
        dude_enqueue(CMD_GO_RIGHT);
        dude_enqueue(CMD_GO_LEFT);
        dude_enqueue(CMD_GO_LEFT);
        dude_enqueue(CMD_GO_DOWN);
        dude_enqueue(CMD_GO_RIGHT);
        dude_enqueue(CMD_GO_DOWN);
    }
    
    if (b_pressed && up_just_pressed)    dude_enqueue(CMD_GO_UP);
    if (b_pressed && down_just_pressed)  dude_enqueue(CMD_GO_DOWN);
    if (b_pressed && left_just_pressed)  dude_enqueue(CMD_GO_LEFT);
    if (b_pressed && right_just_pressed) dude_enqueue(CMD_GO_RIGHT);

    // Cursor
    cursor_update();
    cursor_draw();

    // UI
    set_win_tile_xy(6, 1, '0' + cursor_x);
    set_win_tile_xy(8, 1, '0' + cursor_y);

    char dude_xy_str[8];
    sprintf(dude_xy_str, "%3d,%3d", dude.spr.x, dude.spr.y);
    ui_put_text(6, 2, dude_xy_str);
    
    if (cursor_on_dude) {
        ui_put_text(11, 1, "PRESS A!");
    } else {
        ui_put_text(11, 1, "        ");
    }

    if (cursor_y > 5 && ui_position != UI_TOP) {
        ui_show_window_top();
    } else if (cursor_y < 3 && ui_position != UI_BOTTOM) {
        ui_show_window_bottom();
    }
}