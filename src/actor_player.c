#include "include/actor_player.h"
#include "include/actor_wall.h"
#include "include/actor_particle.h"
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

    return player;
}

void actor_player_draw(actor_T* self)
{
}

void actor_player_tick(actor_T* self)
{
    actor_player_T* player = (actor_player_T*) self;
    state_T* state = get_current_state();

    move(self, self->dx, self->dy);
    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, self->friction); 

    float acceleration = 0.9f;
    float grav_angle = -vec2_angle(
            self->x + self->width/2, self->y + self->height/2, 640/2 + 16/2, 480/2 + 16/2);

    float g_x = self->x + (cos(glm_rad(grav_angle)) * (16*(17/2))) + 16/2;
    float g_y = self->y - (sin(glm_rad(grav_angle)) * (16*(17/2))) + 16/2;

    if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT] || KEYBOARD_STATE->keys[GLFW_KEY_LEFT])
    {
        float move_angle = 0.0f;
        float fix = 3.0f;

        if (KEYBOARD_STATE->keys[GLFW_KEY_RIGHT])
        {
            move_angle = grav_angle + 90.0f + fix;
        }
        else
        {
            move_angle = grav_angle - 90.0f - fix;
        }

        // make character face the angle of the gravity
        self->rz = (atan2(g_y - self->y, g_x - self->x) * 180 / M_PI) - 90.0f;

        self->dx += (cos(glm_rad(move_angle)) * acceleration);
        self->dy -= (sin(glm_rad(move_angle)) * acceleration);
    } 

    if (KEYBOARD_STATE->keys[GLFW_KEY_DOWN])
    {
        physics_vec2_push(&self->dx, &self->dy, 270, acceleration);
    }
    
    actor_T* ground_below = get_wall_at_pos(
        self->x + (cos(glm_rad(grav_angle)) * 1.6f),
        self->y - (sin(glm_rad(grav_angle)) * 1.6f),
        self->width,
        self->height,
        0,
        self->height
    ); 

    if (!ground_below)
    {
        //self->dy += 0.6f;
        physics_vec2_push(&self->dx, &self->dy, grav_angle, 0.6f);
        player->has_emitted_particles = 0;
    }
    else
    {
        actor_wall_T* wall = (actor_wall_T*) ground_below;
        wall->type = WALL_STONE;
        actor_wall_update(wall);

        if (KEYBOARD_STATE->keys[GLFW_KEY_UP])
        {
            self->dx -= (cos(glm_rad(grav_angle)) * acceleration * 10);
            self->dy += (sin(glm_rad(grav_angle)) * acceleration * 10);
            //physics_vec2_push(&self->dx, &self->dy, -grav_angle, acceleration * 20);
        }
        else
        {
            //self->dy = 0;
            //self->dx = 0;
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
