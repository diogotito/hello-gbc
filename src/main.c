#include <gbdk/platform.h>

#include "scene/manager.h"
#include "scene/scn_title.h"
#include "input.h"

void main(void)
{
    // Initialize scene manager with scn_title as the initial scene
	scene_manager_init(&scn_title);

    // Game loop
    while(1) {
        input_refresh();
        scene_process();
        vsync();
    }
}
