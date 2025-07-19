#ifndef __UI_H
#define __UI_H

#include <stdint.h>

#define UI_PALETTE_INDEX 6

void ui_load_gfx(void);
void ui_show_window_top(void);
void ui_draw_panel(uint8_t lines);
void ui_put_text(uint8_t x, uint8_t y, uint8_t w, char *text);

#endif
