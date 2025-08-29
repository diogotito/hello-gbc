#include "manager.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_SCENES 10

int8_t cur_scene_index = -1;
scene_desc scene_stack[MAX_SCENES] = {{0}};
// NOTE: There might be a possible optimization for accessing the current scene
// by storing a copy of the struct in a global variable each time it changes.
// The GBDK docs say it's not optimal to index arrays on the Game Boy.
// However, the scene stack is only indexed once per frame
// (6 at most when switching scenes) so that would be a micro-optimization.

// Deferred returning and calling
bool returning = false;
scene_desc *calling = nullptr;

void scene_manager_init(scene_desc *first_scene) {
    cur_scene_index = 0;
    scene_stack[0] = *first_scene;
    scene_stack[0].init_fn();
}

void scene_process()
{
    // Assumes `scene_manager_init(&first_scene)` was called once
    scene_stack[cur_scene_index].process_fn();
    
    // Perform pending return and call, if any
    if (returning) {
        if (scene_stack[cur_scene_index].finalize_fn)
            scene_stack[cur_scene_index].finalize_fn();
        scene_stack[cur_scene_index].process_fn = nullptr;  // mark as returned
        --cur_scene_index;
        returning = false;
    }
    if (calling) {
        ++cur_scene_index;
        scene_stack[cur_scene_index] = *calling;  // Copy the "stack frame"
        if (scene_stack[cur_scene_index].init_fn)
            scene_stack[cur_scene_index].init_fn();
        calling = nullptr;
    }
}

void scene_call(scene_desc *called_scene)
{
    calling = called_scene;
}

void scene_return()
{
    returning = true;
}

void scene_switch(scene_desc *next_scene)
{
    scene_return();
    scene_call(next_scene);
}
