#include "include/actor_player.h"
#include <coelum/draw_utils.h>
#include <coelum/current.h>
#include <coelum/input.h>
#include <coelum/physics.h>
#include <string.h>


extern keyboard_state_T* KEYBOARD_STATE;


actor_player_T* init_actor_player(float x, float y)
{
    actor_player_T* player = calloc(1, sizeof(struct ACTOR_PLAYER_STRUCT));
    actor_T* actor = (actor_T*) player;
    actor_constructor(actor, x, y, 0.0f, actor_player_tick, actor_player_draw, "player");

    actor->width = 8;
    actor->height = 8;
    actor->friction = 0.3f;

    return player;
}

void actor_player_draw(actor_T* self)
{
    state_T* state = get_current_state();

    draw_positioned_2D_mesh(
        self->x,
        self->y,
        self->z,
        self->width,
        self->height,
        0,
        0,
        255,
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

    actor_T* actor = (void*)0;

    if ((actor = get_wall_at_pos(self->x + xa, self->y + ya, self->width, self->height)) == (void*)0)
    {
        self->x += xa;
        self->y += ya;
    }
    else
    {
        self->dx = 0;
        self->dy = 0;
    }
}

void actor_player_tick(actor_T* self)
{
    move(self, self->dx, self->dy);
    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, self->friction); 

    float acceleration = 0.6f;

    if (KEYBOARD_STATE->keys[GLFW_KEY_LEFT])
    {
        physics_vec2_push(&self->dx, &self->dy, 180, acceleration);
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT])
    {
        physics_vec2_push(&self->dx, &self->dy, 0, acceleration);
    } 

    if (KEYBOARD_STATE->keys[GLFW_KEY_DOWN])
    {
        physics_vec2_push(&self->dx, &self->dy, 270, acceleration);
    }
    
    actor_T* ground_below = get_wall_at_pos(self->x, self->y + self->dy + 0.6f, self->width, self->height); 

    if (!ground_below)
    {
        self->dy += 0.6f;
    }
    else
    {
        if (KEYBOARD_STATE->keys[GLFW_KEY_UP])
        {
            physics_vec2_push(&self->dx, &self->dy, 90, acceleration * 20);
        }
        else
        {
            self->dy = 0;
        }
    } 
}
