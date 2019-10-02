#include "include/actor_player.h"
#include "include/actor_wall.h"
#include "include/actor_particle.h"
#include "include/sprites.h"
#include "include/scene_world.h"
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

static actor_T* get_wall_at_pos(float x, float y, float w, float h, int x_offset, int y_offset)
{
    scene_T* scene = get_current_scene();
    scene_world_T* world = (scene_world_T*) scene;

    int xx = (int) x + x_offset;
    int yy = (int) y + y_offset;
    
    chunk_T* chunk = scene_world_get_chunk_at(world, xx, yy);


    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            actor_wall_T* wall = chunk->walls[i][j];
            actor_T* actor = (actor_T*) wall;

            if (wall->type != WALL_AIR)
            {
                if (x + w > actor->x && x < actor->x + actor->width)
                {
                    if (y + w > actor->y && y < actor->y + actor->height)
                    {
                        return actor;
                    }
                }
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

    actor_T* actor = get_wall_at_pos(self->x + xa, self->y + ya, self->width, self->height, xa > 0 ? self->width : 0, self->height);

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

void actor_player_tick(actor_T* self)
{
    actor_player_T* player = (actor_player_T*) self;
    state_T* state = get_current_state();

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
    
    actor_T* ground_below = get_wall_at_pos(self->x, self->y + self->dy + 0.6f, self->width, self->height, 0, self->height); 

    if (!ground_below)
    {
        self->dy += 0.6f;
        player->has_emitted_particles = 0;
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

        if (!player->has_emitted_particles)
        {
            for (int i = 0; i < random_int(3, 10); i++)
            {
                actor_particle_T* particle = init_actor_particle(self->x, self->y + self->height);
                actor_T* ap = (actor_T*) particle;
                ap->y -= ap->height;
                physics_vec2_push(&ap->dx, &ap->dy, random_int(0, 180), random_int(3, 5));
                dynamic_list_append(state->actors, ap);
            }

            player->has_emitted_particles = 1;
        }
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
