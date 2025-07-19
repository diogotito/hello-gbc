#include "ui.h"
#include "../res/ui_tiles.h"
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/isr.h>
#include <stdint.h>

#define ui_tiles_corner_tile     (ui_tiles_TILE_ORIGIN + 1)
#define ui_tiles_horizontal_tile (ui_tiles_TILE_ORIGIN + 2)
#define ui_tiles_vertical_tile   (ui_tiles_TILE_ORIGIN + 3)
#define ui_tiles_fill_tile       (ui_tiles_TILE_ORIGIN + 4)

void draw_panel(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

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
}

void lyc_isr_c(void) INTERRUPT CRITICAL PRESERVES_REGS(b, c, d, e, h, l)
{
    HIDE_WIN;
}

void lyc_isr(void) INTERRUPT CRITICAL NAKED PRESERVES_REGS(b, c, d, e, h, l)
{
    // This is the generated assembly code for HIDE_WIN.
    // It only touches the A register, so there's no need to push BC, DE and HL
    // to the stack. Doing so would take too much time as there's only 22 CPU
    // cycles to turn the window layer off before the line starts to be rendered!
    __asm
    push af
    ldh	a, (_LCDC_REG)
	and	a, #0xdf
	ldh	(_LCDC_REG), a
    pop af
    reti
    __endasm;
}

void vbl_isr(void)
{
    SHOW_WIN;
}

void ui_draw_panel(uint8_t lines)
{
    draw_panel(0, 0, 20, lines + 2);
}

void ui_show_window_top()
{
    move_win(7, 0); // or y = 8 * (18 - 4) to show it on the bottom
    
    // Setup interrupt for hiding window when LY = 4 * 8
    LYC_REG = 4 * 8;
    CRITICAL {
        STAT_REG = STATF_LYC;
        ISR_VECTOR(VECTOR_STAT, lyc_isr);
        add_VBL(vbl_isr);
    }
    set_interrupts(IE_REG | IEF_STAT | IEF_VBLANK);

    // Setup interrupt for showing the window at the top of the frame
    SHOW_WIN;
}

void ui_put_text(uint8_t x, uint8_t y, uint8_t w, char* text)
{
    
    // strcpy(&_SCRN1[y * DEVICE_SCREEN_BUFFER_WIDTH + x], text); // Prone to tile corruption!
    set_win_tiles(x, y, w, 1, text);
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