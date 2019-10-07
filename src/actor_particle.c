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

extern unsigned int SHADER_COLORED;


actor_particle_T* init_actor_particle(float x, float y)
{
    actor_particle_T* particle = calloc(1, sizeof(struct ACTOR_PARTICLE_STRUCT));
    actor_entity_T* entity = (actor_entity_T*) particle;
    actor_entity_constructor(entity);
    actor_T* actor = (actor_T*) entity;
    actor_constructor(actor, x, y, 0.2f, actor_particle_tick, actor_particle_draw, "particle");

    int size = random_int(4, 6);
    actor->width = size;
    actor->height = size;

    particle->r = 255;
    particle->g = 0;
    particle->b = 0;
    particle->timer = 0;
    actor->shader_program = SHADER_COLORED;

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
    actor_entity_T* entity = (actor_entity_T*) self;

    move(entity, self->dx, self->dy);
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
}
