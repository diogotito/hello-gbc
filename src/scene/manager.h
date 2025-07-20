#ifndef __SCENE_MANAGER_H
#define __SCENE_MANAGER_H

typedef struct scene_desc {
    void (*process_fn)();
    void (*init_fn)();
    void (*finalize_fn)();
    char id[6];
} scene_desc;

void scene_process();
void scene_call(scene_desc*);
void scene_return();
void scene_switch(scene_desc*);

#endif
