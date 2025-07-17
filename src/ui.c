#include "ui.h"
#include "../res/frames.h"
#include <gb/gb.h>
#include <gb/isr.h>

#define frames_corner_tile     (frames_TILE_ORIGIN + 0)
#define frames_horizontal_tile (frames_TILE_ORIGIN + 1)
#define frames_vertical_tile   (frames_TILE_ORIGIN + 2)
#define frames_fill_tile       (frames_TILE_ORIGIN + 3)

void draw_panel(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void ui_load_gfx(void)
{
    // Load "frames" tiles to VRAM and palette to CRAM
    set_win_data(frames_TILE_ORIGIN, frames_TILE_COUNT, frames_tiles);
    set_bkg_palette(UI_PALETTE_INDEX, frames_PALETTE_COUNT, frames_palettes);

    // Fill window attribute map with priority bit (7) + "frames" color palette
    VBK_REG = VBK_ATTRIBUTES;
    fill_win_rect(0, 0, 32, 32, (1 << 7) | UI_PALETTE_INDEX);
    VBK_REG = VBK_TILES;
}

void lyc_isr_c(void) INTERRUPT CRITICAL PRESERVES_REGS(b, c, d, e, h, l)
{
    HIDE_WIN;
}

void lyc_isr(void) INTERRUPT CRITICAL NAKED PRESERVES_REGS(b, c, d, e, h, l)
{
    __asm
    push af
    ldh	a, (_LCDC_REG + 0)
	and	a, #0xdf
	ldh	(_LCDC_REG + 0), a
    pop af
    reti
    __endasm;
}

void vbl_isr(void)
{
    SHOW_WIN;
}

void ui_show_window()
{
    // Draw a panel
    draw_panel(0, 0, 20, 4);
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

void draw_panel(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    // Corners
    set_win_tile_xy(x, y, frames_corner_tile);
    set_win_tile_xy(x + width - 1, y, frames_corner_tile);
    set_win_tile_xy(x, y + height - 1, frames_corner_tile);
    set_win_tile_xy(x + width - 1, y + height - 1, frames_corner_tile);

    // Borders
    fill_win_rect(x + 1, y, width - 2, 1, frames_horizontal_tile);
    fill_win_rect(x + 1, y + height - 1, width - 2, 1, frames_horizontal_tile);
    fill_win_rect(x, y + 1, 1, height - 2, frames_vertical_tile);
    fill_win_rect(x + width - 1, y + 1, 1, height - 2, frames_vertical_tile);

    // Fill
    fill_win_rect(x + 1, y + 1, width - 2, height - 2, frames_fill_tile);
}