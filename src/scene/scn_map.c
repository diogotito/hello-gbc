#include <stdio.h>
#include "scn_map.h"
#include "../../res/ruins__map_desc.h"
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

uint8_t unit_sprites_count = 2;
unit_spr unit_sprites[MAX_UNITS_IN_MAP] = {
    {{.frame = 0, .anim = 0, .flipX = false, .x = 112, .y = 32}, UNIT_WAITING, 0},
    {{.frame = 0, .anim = 0, .flipX = false, .x =  64, .y = 64}, UNIT_WAITING, 1},
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
    // Load the "ruins" map
    map_init(&ruins_map_desc);
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

    // Init unit command queues
    for (uint8_t i = 0; i < unit_sprites_count; i++) {
        unit_init_queue(unit_sprites[i].id);
    }
    
}

void scn_map_process(void)
{
    // Unit
    for (size_t i = 0; i < unit_sprites_count; i++) {
        unit_spr_update(&unit_sprites[i]);
        unit_spr_draw(&unit_sprites[i]);

        if (cursor_on_unit == i && a_just_pressed) {
            unit_enqueue(i, CMD_GO_UP);
            unit_enqueue(i, CMD_GO_RIGHT);
            unit_enqueue(i, CMD_GO_LEFT);
            unit_enqueue(i, CMD_GO_LEFT);
            unit_enqueue(i, CMD_GO_DOWN);
            unit_enqueue(i, CMD_GO_RIGHT);
            unit_enqueue(i, CMD_GO_DOWN);
        }
        
        if (b_pressed && up_just_pressed)    unit_enqueue(i, CMD_GO_UP);
        if (b_pressed && down_just_pressed)  unit_enqueue(i, CMD_GO_DOWN);
        if (b_pressed && left_just_pressed)  unit_enqueue(i, CMD_GO_LEFT);
        if (b_pressed && right_just_pressed) unit_enqueue(i, CMD_GO_RIGHT);
    }

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