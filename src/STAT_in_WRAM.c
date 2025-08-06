#include "STAT_in_WRAM.h"
#include <gbdk/platform.h>
#include <gb/isr.h>

void lyc_isr(void) INTERRUPT CRITICAL NAKED PRESERVES_REGS(b, c, d, e, h, l)
{
    // This is the generated assembly code for HIDE_WIN.
    // It only touches the A register, so there's no need to push BC, DE and HL
    // to the stack. Doing so would take too much time as there's only 22 CPU
    // cycles to turn the window layer off before the line starts to get rendered!
    __asm
    push af
    ldh	a, (_LCDC_REG)
	and	a, #~LCDCF_WINON
	ldh	(_LCDC_REG), a
    pop af
    reti
    __endasm;
}

ISR_VECTOR(VECTOR_STAT, lyc_isr)
