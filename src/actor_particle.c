#include "include/actor_particle.h"
#include "include/actor_wall.h"
#include "include/sprites.h"
#include <coelum/draw_utils.h>
#include <coelum/current.h>
#include <coelum/physics.h>
#include <coelum/utils.h>
#include <string.h>


extern const sprite_T* SPRITE_STONE;
extern const sprite_T* SPRITE_GRASS;


actor_particle_T* init_actor_particle(float x, float y)
{
    actor_particle_T* particle = calloc(1, sizeof(struct ACTOR_PARTICLE_STRUCT));
    actor_T* actor = (actor_T*) particle;
    actor_constructor(actor, x, y, 0.2f, actor_particle_tick, actor_particle_draw, "particle");

    int size = random_int(2, 6);
    actor->width = size;
    actor->height = size;

    particle->r = 255;
    particle->g = 0;
    particle->b = 0;
    particle->timer = 0;

    return particle;
}

void actor_particle_draw(actor_T* self)
{
    actor_particle_T* particle = (actor_particle_T*) self;

    state_T* state = get_current_state();

    draw_positioned_2D_mesh(
        self->x,
        self->y,
        self->z,
        self->width,
        self->height,
        particle->r,
        particle->g,
        particle->b,
        1.0f,
        state     
    );
}

static actor_T* get_wall_at_pos(float x, float y, float w, float h)
{
    state_T* state = get_current_state();

    for (int i = 0; i < state->actors->size; i++)
    {
        actor_T* actor = (actor_T*) state->actors->items[i];

        if (strcmp(actor->type_name, "wall") != 0)
            continue;

        if (x + w > actor->x && x < actor->x + actor->width)
        {
            if (y + w > actor->y && y < actor->y + actor->height)
            {
                return actor;
            }
        }
    }

    return (void*) 0;
}

static void move(actor_T* self, float xa, float ya)
{
    if (xa != 0 && ya != 0)
    {
        move(self, xa, 0);
        move(self, 0, ya);
        return;
    }

    actor_T* actor = get_wall_at_pos(self->x + xa, self->y + ya, self->width, self->height);

    if (actor == (void*)0)
    {
        self->x += xa;
        self->y += ya;
    }
    else
    {
        actor_wall_T* wall = (actor_wall_T*) actor;
        wall->r = 0;

        self->dx = 0;
        self->dy = 0;
    }
}

void actor_particle_tick(actor_T* self)
{
    move(self, self->dx, self->dy);
    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, self->friction); 
    
    state_T* state = get_current_state();
    actor_particle_T* particle = (actor_particle_T*) self;

    if (particle->timer < 20)
    {
        particle->timer += 1;
    }
    else
    {
        dynamic_list_remove(state->actors, self, (void*)0);
    }

    actor_T* ground_below = get_wall_at_pos(self->x, self->y + self->dy + 0.6f, self->width, self->height); 

    if (!ground_below)
    {
        self->dy += 0.6f;
    }
    else
    {
        self->dy = 0.0f;
    }
}
