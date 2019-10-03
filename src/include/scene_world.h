#ifndef SCENE_WORLD_H
#define SCENE_WORLD_H
#include <coelum/scene.h>
#include "chunk.h"
#define NR_CHUNKS 24


typedef struct SCENE_WORLD_STRUCT
{
    scene_T base;

    chunk_T* chunks[NR_CHUNKS][NR_CHUNKS];
} scene_world_T;

scene_world_T* init_scene_world();

void scene_world_tick(scene_T* self);

void scene_world_draw(scene_T* self);

chunk_T* scene_world_get_chunk_at(scene_world_T* world, int x, int y);

void scene_world_generate(scene_world_T* self);
#endif
