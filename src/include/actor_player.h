#ifndef ACTOR_PLAYER_H
#define ACTOR_PLAYER_H
#include "actor_entity.h"


typedef struct ACTOR_PLAYER_STRUCT
{
    actor_entity_T base;
    unsigned int has_emitted_particles;
    actor_T* vehicle;
} actor_player_T;


actor_player_T* init_actor_player(float x, float y);

void actor_player_draw(actor_T* self);

void actor_player_tick(actor_T* self);
#endif
