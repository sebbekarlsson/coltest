#include "include/actor_wall.h"
#include <coelum/draw_utils.h>
#include <coelum/current.h>


actor_wall_T* init_actor_wall(float x, float y)
{
    actor_wall_T* wall = calloc(1, sizeof(struct ACTOR_WALL_STRUCT));
    actor_T* actor = (actor_T*) wall;
    actor_constructor(actor, x, y, 0.0f, actor_wall_tick, actor_wall_draw, "wall");

    actor->width = 16;
    actor->height = 16;

    return wall;
}

void actor_wall_draw(actor_T* self)
{
    state_T* state = get_current_state();

    draw_positioned_2D_mesh(
        self->x,
        self->y,
        self->z,
        self->width,
        self->height,
        255,
        0,
        0,
        1.0f,
        state
    );
}

void actor_wall_tick(actor_T* self)
{
}
