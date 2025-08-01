#include <stdio.h>
#include "scn_map.h"
#include "../game/unit.h"
#include "../game/map.h"
#include "../ui.h"

// -----------------
// Scene description
// -----------------

void scn_map_process(void);
void scn_map_init(void);

const scene_desc scn_map = {
    .process_fn = scn_map_process,
    .init_fn = scn_map_init,
    .finalize_fn = nullptr,
    .id = {'M', 'A', 'P', ' ', ' ', ' '},
};

// -----------
// Scene state
// -----------

unit_spr unit = {
    {.frame = 0, .anim = 0, .flipX = false, .x = 112, .y = 32},
    UNIT_WAITING
};

// --------
// Partials
// --------

#include "scn_map__cursor__.c"

// ---------------
// Scene lifecycle
// ---------------

void scn_map_init(void)
{
    map_init();
    map_load_gfx();

    // Turn the background map on to make it visible
    SHOW_BKG;

    // Load unit sprite
    unit_load_gfx();
    static const palette_color_t black_palette[] = {RGB8(0, 0, 0), RGB8(159, 159, 159), RGB8(255, 0, 0), RGB8(0, 0, 0)};
    set_sprite_palette(1, 1, black_palette);
    SPRITES_8x8;
    SHOW_SPRITES;

    // Load cursor
    cursor_load_gfx();

    // Load and set up UI
    ui_load_gfx();
    ui_draw_panel(2);
    ui_put_text(1, 1, "CUR@( , )");
    ui_put_text(1, 2, "GUY@(   ,   )");
    ui_show_window_top();
}

void scn_map_process(void)
{
    // Unit
    unit_update(&unit);
    unit_draw(&unit);
    
    if (cursor_on_unit && a_just_pressed) {
        unit_enqueue(CMD_GO_UP);
        unit_enqueue(CMD_GO_RIGHT);
        unit_enqueue(CMD_GO_LEFT);
        unit_enqueue(CMD_GO_LEFT);
        unit_enqueue(CMD_GO_DOWN);
        unit_enqueue(CMD_GO_RIGHT);
        unit_enqueue(CMD_GO_DOWN);
    }
    
    if (b_pressed && up_just_pressed)    unit_enqueue(CMD_GO_UP);
    if (b_pressed && down_just_pressed)  unit_enqueue(CMD_GO_DOWN);
    if (b_pressed && left_just_pressed)  unit_enqueue(CMD_GO_LEFT);
    if (b_pressed && right_just_pressed) unit_enqueue(CMD_GO_RIGHT);

    // Cursor
    cursor_update();
    cursor_draw();

    // UI
    if (cursor_y > 5 && ui_position != UI_TOP) {
        ui_show_window_top();
    } else if (cursor_y < 3 && ui_position != UI_BOTTOM) {
        ui_show_window_bottom();
    }
}