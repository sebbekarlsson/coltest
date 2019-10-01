#ifndef CHUNK_H
#define CHUNK_H
#include "actor_wall.h"

typedef struct CHUNK_STRUCT
{
    actor_wall_T* walls[8][8];
} chunk_T;

chunk_T* init_chunk();

void chunk_tick(chunk_T* self);

void chunk_draw(chunk_T* self, int x, int y);
#endif
