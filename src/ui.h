#ifndef __UI_H
#define __UI_H

#include <stdint.h>

#define UI_PALETTE_INDEX 6

enum ui_win_pos {
    UI_HIDDEN,
    UI_TOP,
    UI_BOTTOM
};

extern enum ui_win_pos ui_position;

void ui_load_gfx(void);
void ui_show_window_top(void);
void ui_show_window_bottom(void);
void ui_draw_panel(uint8_t lines);

void ui_put_text(uint8_t x, uint8_t y, uint8_t w, char *text);

#define ui_put_text(x, y, string) set_win_tiles((uint8_t)(x), (uint8_t)(y), sizeof(string) - 1, 1, string);

#endif
