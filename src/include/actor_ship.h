#ifndef ACTOR_SHIP_H
#define ACTOR_SHIP_H
#include "actor_entity.h"


typedef struct ACTOR_SHIP_STRUCT
{
    actor_entity_T base;
    actor_T* rider;
} actor_ship_T;


actor_ship_T* init_actor_ship(float x, float y);

void actor_ship_draw(actor_T* self);

void actor_ship_tick(actor_T* self);
#endif
