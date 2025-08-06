/**
 * @file manager.h
 * A stack of function pointers for structuring the game flow.
 * Each "scene" might correspond to a game screen
 * and can call or return to other "scenes".
 */
#ifndef __SCENE_MANAGER_H
#define __SCENE_MANAGER_H

typedef struct scene_desc {
    void (*process_fn)();
    void (*init_fn)();
    void (*finalize_fn)();
    char id[6];
} scene_desc;

/**
 * Initializes the scene stack and sets the first scene.
 *
 * Call this once before `scene_process()` ever gets called.
 *
 * @param first_scene The first scene in the scene stack.
 */
void scene_manager_init(scene_desc *first_scene);

/**
 * Call this every frame from main().
 * Delegates to the current scene's `process_fn` function pointer.
 *
 * @attention The scene stack must be initialized by `scene_manager_init`
 * before this function is called.
 */
void scene_process();

/**
 * Push param next_scene into the scene stack.
 */
void scene_call(scene_desc *next_scene);

/**
 * Pops the current scene from the scene stack
 * and yields processing to the callee scene.
 */
void scene_return();

/**
 * Switches to a different scene in place.
 */
void scene_switch(scene_desc*);

#endif
