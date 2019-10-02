#include "include/actor_particle.h"
#include "include/actor_wall.h"
#include "include/sprites.h"
#include "include/collision.h"
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

    actor_T* ground_below = get_wall_at_pos(self->x, self->y + self->dy + 0.6f, self->width, self->height, self->height, 0); 

    if (!ground_below)
    {
        self->dy += 0.6f;
    }
    else
    {
        self->dy = 0.0f;
    }
}
