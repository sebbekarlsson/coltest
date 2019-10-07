#include "include/actor_player.h"
#include "include/actor_wall.h"
#include "include/actor_particle.h"
#include "include/actor_ship.h"
#include "include/sprites.h"
#include "include/scene_world.h"
#include "include/collision.h"
#include <coelum/draw_utils.h>
#include <coelum/current.h>
#include <coelum/input.h>
#include <coelum/physics.h>
#include <coelum/utils.h>
#include <string.h>


extern keyboard_state_T* KEYBOARD_STATE;


actor_player_T* init_actor_player(float x, float y)
{
    actor_player_T* player = calloc(1, sizeof(struct ACTOR_PLAYER_STRUCT));
    actor_T* actor = (actor_T*) player;
    actor_constructor(actor, x, y, 0.0f, actor_player_tick, actor_player_draw, "player");

    actor->width = 16;
    actor->height = 16;
    actor->friction = 0.3f;

    actor->sprite = SPRITE_PLAYER_LEFT;

    player->has_emitted_particles = 0;
    player->vehicle = (void*) 0;

    return player;
}

void actor_player_draw(actor_T* self)
{
    state_T* state = get_current_state();
    float g_x = self->x + (cos(glm_rad(-(self->rz+90.0f) + 360)) * (16*(17/2))) + 16/2;
    float g_y = self->y - (sin(glm_rad(-(self->rz+90.0f) + 360)) * (16*(17/2))) + 16/2;

    draw_line(
            self->x,
            self->y,
            self->z,
            g_x,
            g_y,
            self->z,
            0,
            0,
            255,
            state        
            );
}

void actor_player_tick(actor_T* self)
{
    actor_player_T* player = (actor_player_T*) self;
    state_T* state = get_current_state();

    move(self, self->dx, self->dy);
    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, self->friction); 

    if (player->vehicle)
    {
        self->x = player->vehicle->x + player->vehicle->width / 2;
        self->y = player->vehicle->y + player->vehicle->height / 2;
        actor_ship_T* ship = (actor_ship_T*) player->vehicle;
        ship->rider = self;
    }

    float acceleration = 0.9f;

    float g_x = self->x + (cos(glm_rad(-(self->rz+90.0f) + 360)) * (16*(17/2))) + 16/2;
    float g_y = self->y - (sin(glm_rad(-(self->rz+90.0f) + 360)) * (16*(17/2))) + 16/2;

    float gravity_angle = -vec2_angle(
            self->x + self->width/2, self->y + self->height/2, g_x, g_y);

    if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT] || KEYBOARD_STATE->keys[GLFW_KEY_LEFT])
    {
        if (!player->vehicle)
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
    } 

    actor_T* ground_below = get_wall_at_pos(
            self->x + (cos(glm_rad(gravity_angle)) * 1.6f),
            self->y - (sin(glm_rad(gravity_angle)) * 1.6f),
            self->width,
            self->height,
            0,
            self->height
            ); 

    if (!ground_below)
    {
        player->has_emitted_particles = 0;
    }
    else
    {
        if (KEYBOARD_STATE->keys[GLFW_KEY_UP] && !player->vehicle) // jump
            physics_vec2_push(&self->dx, &self->dy, gravity_angle, -(acceleration * 10));

        if (KEYBOARD_STATE->keys[GLFW_KEY_SPACE] && !player->vehicle) // jump
            physics_vec2_push(&self->dx, &self->dy, gravity_angle, -(acceleration * 20));

        if (!player->has_emitted_particles)
        {
            for (int i = 0; i < random_int(3, 10); i++)
            {
                actor_particle_T* particle = init_actor_particle(self->x, self->y + self->height);
                actor_T* ap = (actor_T*) particle;
                ap->y -= ap->height;
                physics_vec2_push(&ap->dx, &ap->dy, gravity_angle - 90 - random_int(0, 180), random_int(3, 5));
                dynamic_list_append(state->actors, ap);
            }

            player->has_emitted_particles = 1;
        }
    }

    actor_T* near_vehicle = (void*)0;

    for (int i = 0; i < state->actors->size; i++)
    {
        actor_T* a = (actor_T*) state->actors->items[i];

        if (strcmp(a->type_name, "ship") == 0)
        {
            if (vec2_distance(
                self->x + self->width / 2,
                self->y + self->height / 2,
                a->x + a->width / 2,
                a->y + a->height / 2
            ) < a->width + self->width)
            {
                near_vehicle = a;
                break;
            }
            else
            {

                actor_ship_T* ship = (actor_ship_T*) a;
                ship->rider = (void*) 0;
            }
        }
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_ENTER] && !KEYBOARD_STATE->key_locks[GLFW_KEY_ENTER])
    {
        if (near_vehicle && !player->vehicle)
        {
            player->vehicle = near_vehicle;
        }
        else
        {
            player->vehicle = (void*)0;
        }

        KEYBOARD_STATE->key_locks[GLFW_KEY_ENTER] = 1;
    }

    if (self->dx > 0 || self->dx < 0)
    {
        self->sprite->animate = 1;
    }
    else
    {
        self->sprite->animate = 0;
    }
}
