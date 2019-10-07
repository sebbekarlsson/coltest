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
    actor_T* actor = (actor_T*) ship;
    actor_constructor(actor, x, y, 0.3f, actor_ship_tick, actor_ship_draw, "ship");

    ship->rider = (void*)0;

    actor->width = 128;
    actor->height = 128;
    actor->friction = 0.3f;
    actor->sprite = SPRITE_SHIP;

    return ship;
}

void actor_ship_draw(actor_T* self)
{
}

void actor_ship_tick(actor_T* self)
{
    actor_ship_T* ship = (actor_ship_T*) self;

    move(self, self->dx, self->dy);
    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, self->friction);

    float g_x = self->x + (cos(glm_rad(-(self->rz+90.0f) + 360)) * (16*(17/2))) + 16/2;
    float g_y = self->y - (sin(glm_rad(-(self->rz+90.0f) + 360)) * (16*(17/2))) + 16/2;

    float gravity_angle = -vec2_angle(
            self->x + self->width/2, self->y + self->height/2, g_x, g_y);

    float acceleration = 0.9f;

    if (ship->rider)
    {
        if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT] || KEYBOARD_STATE->keys[GLFW_KEY_LEFT])
        {
            float move_angle = 0.0f;
            float fix = 3.0f;

            if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT])
                move_angle = gravity_angle + 90.0f + fix;
            else // left
                move_angle = gravity_angle - 90.0f - fix;

            self->dx += (cos(glm_rad(move_angle)) * acceleration);
            self->dy -= (sin(glm_rad(move_angle)) * acceleration);
        }

        if (KEYBOARD_STATE->keys[GLFW_KEY_UP])
        {
            self->dx -= (cos(glm_rad(gravity_angle)) * acceleration);
            self->dy += (sin(glm_rad(gravity_angle)) * acceleration);
        }
    }
}
