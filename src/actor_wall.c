#include "include/actor_wall.h"
#include "include/sprites.h"
#include <coelum/draw_utils.h>
#include <coelum/current.h>


extern const sprite_T* SPRITE_STONE;
extern const sprite_T* SPRITE_GRASS;


actor_wall_T* init_actor_wall(float x, float y, int type)
{
    actor_wall_T* wall = calloc(1, sizeof(struct ACTOR_WALL_STRUCT));
    actor_T* actor = (actor_T*) wall;
    actor_constructor(actor, x, y, 0.0f, actor_wall_tick, actor_wall_draw, "wall");

    actor->width = 16;
    actor->height = 16;

    wall->r = 255;
    wall->g = 0;
    wall->b = 0;
    wall->type = type;

    switch (type)
    {
        case WALL_STONE: actor->sprite = SPRITE_STONE; break;
        case WALL_GRASS: actor->sprite = SPRITE_GRASS; break;
    }

    return wall;
}

void actor_wall_draw(actor_T* self)
{
}

void actor_wall_tick(actor_T* self)
{
}
