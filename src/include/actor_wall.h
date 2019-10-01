#ifndef ACTOR_WALL_H
#define ACTOR_WALL_H
#include <coelum/actor.h>


typedef struct ACTOR_WALL_STRUCT
{
    actor_T base;
    float r;
    float g;
    float b;

    enum
    {
        WALL_STONE,
        WALL_GRASS,
        WALL_AIR
    } type;
} actor_wall_T;


actor_wall_T* init_actor_wall(float x, float y, int type);

void actor_wall_draw(actor_T* self);

void actor_wall_tick(actor_T* self);

void actor_wall_update(actor_wall_T* self);
#endif
