#ifndef ACTOR_PARTICLE_H
#define ACTOR_PARTICLE_H
#include <coelum/actor.h>


typedef struct ACTOR_PARTICLE_STRUCT
{
    actor_T base;
    float r;
    float g;
    float b;
    int timer;
} actor_particle_T;


actor_particle_T* init_actor_particle(float x, float y);

void actor_particle_draw(actor_T* self);

void actor_particle_tick(actor_T* self);
#endif
