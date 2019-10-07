#ifndef ACTOR_ENTITY_H
#define ACTOR_ENTITY_H
#include <coelum/actor.h>
#include "actor_planet.h"

typedef struct ACTOR_ENTITY_STRUCT
{
    actor_T base;
    actor_planet_T* planet;
    float gravity_angle;
    float g_x;
    float g_y;
    unsigned int is_on_ground;
    unsigned int should_face_gravity;
} actor_entity_T;

actor_entity_T* actor_entity_constructor(actor_entity_T* actor_entity);
#endif
