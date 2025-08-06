#include "ui.h"
#include "../res/ui_tiles.h"
#include <gbdk/platform.h>
#include <gb/isr.h>
#include <stdint.h>

#define ui_tiles_corner_tile     (ui_tiles_TILE_ORIGIN + 1)
#define ui_tiles_horizontal_tile (ui_tiles_TILE_ORIGIN + 2)
#define ui_tiles_vertical_tile   (ui_tiles_TILE_ORIGIN + 3)
#define ui_tiles_fill_tile       (ui_tiles_TILE_ORIGIN + 4)

// Keep track of current window height,
// to disable the WIN layer at the right scanline when the UI is set up to be
// shown at the top of the screen.
uint8_t current_window_height = 8;

enum ui_win_pos ui_position = UI_TOP;

void vbl_isr(void)
{
    if (ui_position != UI_HIDDEN) {
        SHOW_WIN;
    }
}

void ui_load_gfx(void)
{
    // Switch to CGB's VRAM Bank 1 to load UI tile data separate from level
    // and sprite tiles, and initialize the whole window attribute map while we
    // are at it.
    VBK_REG = VBK_BANK_1;

    // Window decorations, UI elements, font and symbols go to to indices 0-127
    set_win_data(ui_tiles_TILE_ORIGIN, ui_tiles_TILE_COUNT, ui_tiles_tiles);

    // Load the 2 UI palettes to CRAM
    set_bkg_palette(UI_PALETTE_INDEX, ui_tiles_PALETTE_COUNT, ui_tiles_palettes);

    // Fill window attribute map with priority (bit 7) so that OBJs don't invade
    // and use VRAM Bank 1 and the first UI palette for all tiles (bit 3).
    fill_win_rect(0, 0, 32, 32, S_PRIORITY | S_BANK | UI_PALETTE_INDEX);

    VBK_REG = VBK_BANK_0;

    // Setup ISRs for rendering the window on top
    CRITICAL {
        STAT_REG = STATF_LYC;
        add_VBL(vbl_isr);
    }
}

void ui_show_window_top()
{
    move_win(7, 0);
    
    // Setup interrupt for hiding window when LY = current_window_height
    LYC_REG = current_window_height;
    set_interrupts(IE_REG | IEF_STAT);

    ui_position = UI_TOP;
}

void ui_show_window_bottom()
{
    move_win(7, 8 * (18 - 4));

    // Disable the interrupt for LY = current_window_height
    set_interrupts(IE_REG & ~IEF_STAT);

    // Hide window for the remaining of this frame to avoid flashing the screen
    // with the window tilemap. It gets shown by the vblank handler.
    HIDE_WIN;
    
    ui_position = UI_BOTTOM;
}

void ui_hide_window()
{
    set_interrupts(IE_REG & ~IEF_STAT);
    HIDE_WIN;
    ui_position = UI_HIDDEN;
}

void draw_panel(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    // Flip some tiles and switch palette for the panel body
    VBK_REG = VBK_ATTRIBUTES;
    fill_win_rect(x + width - 1, y + 1, 1, height - 2, S_FLIPX | S_PRIORITY | S_BANK | UI_PALETTE_INDEX);
    fill_win_rect(x + 1, y + height - 1, width - 2, 1, S_FLIPY | S_PRIORITY | S_BANK | UI_PALETTE_INDEX);
    fill_win_rect(x + 1, y + 1, width - 2, height - 2, S_PRIORITY | S_BANK | (UI_PALETTE_INDEX + 1));
    VBK_REG = VBK_TILES;

    // Corners
    set_win_tile_xy(x, y, ui_tiles_corner_tile);
    set_win_tile_xy(x + width - 1, y, ui_tiles_corner_tile);
    set_win_tile_xy(x, y + height - 1, ui_tiles_corner_tile);
    set_win_tile_xy(x + width - 1, y + height - 1, ui_tiles_corner_tile);

    // Borders
    fill_win_rect(x + 1, y, width - 2, 1, ui_tiles_horizontal_tile);
    fill_win_rect(x + 1, y + height - 1, width - 2, 1, ui_tiles_horizontal_tile);
    fill_win_rect(x, y + 1, 1, height - 2, ui_tiles_vertical_tile);
    fill_win_rect(x + width - 1, y + 1, 1, height - 2, ui_tiles_vertical_tile);

    // Fill
    fill_win_rect(x + 1, y + 1, width - 2, height - 2, ui_tiles_fill_tile);
}

void ui_draw_panel(uint8_t lines)
{
    current_window_height = 8 * (lines + 2);
    draw_panel(0, 0, 20, lines + 2);
}