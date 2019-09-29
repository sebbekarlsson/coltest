#ifndef ACTOR_PLAYER_H
#define ACTOR_PLAYER_H
#include <coelum/actor.h>


typedef struct ACTOR_PLAYER_STRUCT
{
    actor_T base;
} actor_player_T;


actor_player_T* init_actor_player(float x, float y);

void actor_player_draw(actor_T* self);

void actor_player_tick(actor_T* self);
#endif
