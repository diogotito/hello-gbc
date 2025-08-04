#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/emu_debug.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "scene/manager.h"
#include "scene/scn_title.h"
#include "input.h"

void main(void)
{
	scene_first(&scn_title);

    while(1) {
        input_refresh();
        scene_process();
        vsync();
    }
}
