#ifndef ACTOR_PLANET_H
#define ACTOR_PLANET_H
#include <coelum/actor.h>
#include "scene_world.h"


typedef struct ACTOR_PLANET_STRUCT
{
    actor_T base;
    scene_world_T* world;
    int mass_radius;
    int gravity_radius;
    unsigned int is_generated;
} actor_planet_T;

actor_planet_T* init_actor_planet(float x, float y, scene_world_T* world, int mass_radius, int gravity_radius);

void actor_planet_generate(actor_planet_T* self);

void actor_planet_tick(actor_T* self);

void actor_planet_draw(actor_T* self);
#endif
