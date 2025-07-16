#include "ui.h"
#include "../res/frames.h"
#include <gb/gb.h>

void ui_load_gfx(void)
{
    set_win_data(frames_TILE_ORIGIN, frames_TILE_COUNT, frames_tiles);
    set_bkg_palette(UI_PALETTE_INDEX, frames_PALETTE_COUNT, frames_palettes);
}