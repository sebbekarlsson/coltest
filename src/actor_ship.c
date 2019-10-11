#include "include/actor_ship.h"
#include "include/actor_wall.h"
#include "include/sprites.h"
#include "include/collision.h"
#include <coelum/draw_utils.h>
#include <coelum/current.h>
#include <coelum/physics.h>
#include <coelum/utils.h>
#include <coelum/input.h>
#include <string.h>


extern const sprite_T* SPRITE_SHIP;

extern unsigned int SHADER_COLORED;

extern keyboard_state_T* KEYBOARD_STATE;


actor_ship_T* init_actor_ship(float x, float y)
{
    actor_ship_T* ship = calloc(1, sizeof(struct ACTOR_SHIP_STRUCT));
    actor_entity_T* entity = (actor_entity_T*) ship;
    actor_entity_constructor(entity);
    actor_T* actor = (actor_T*) entity;
    actor_constructor(actor, x, y, 0.3f, actor_ship_tick, actor_ship_draw, "ship");

    ship->rider = (void*)0;

    actor->width = 84;
    actor->height = 58;
    actor->friction = 0.3f;
    actor->sprite = SPRITE_SHIP;

    return ship;
}

void actor_ship_draw(actor_T* self)
{
}

void actor_ship_tick(actor_T* self)
{
    actor_entity_T* entity = (actor_entity_T*) self;
    actor_ship_T* ship = (actor_ship_T*) self;

    move(entity, self->dx, self->dy);
    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, self->friction);

    float acceleration = 0.9f;

    if (ship->rider)
    {
        if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT] || KEYBOARD_STATE->keys[GLFW_KEY_LEFT])
        {
            if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT])
                self->rz += 1;
            else // left
                self->rz -= 1;
        }

        if (KEYBOARD_STATE->keys[GLFW_KEY_UP])
        {
            self->dx += (sin(glm_rad(self->rz)) * acceleration);
            self->dy -= (cos(glm_rad(self->rz)) * acceleration);
        }
    }
}
