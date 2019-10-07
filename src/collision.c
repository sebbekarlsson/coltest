#include "include/collision.h"
#include "include/scene_world.h"
#include "include/actor_planet.h"
#include <string.h>
#include <coelum/current.h>
#include <coelum/utils.h>
#include <coelum/physics.h>


actor_T* get_wall_at_pos(float x, float y, float w, float h, int x_offset, int y_offset)
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

void move(actor_T* self, float xa, float ya)
{
    if (xa != 0 && ya != 0)
    {
        move(self, xa, 0);
        move(self, 0, ya);
        return;
    }

    actor_T* actor = get_wall_at_pos(
        self->x + xa,
        self->y + ya,
        self->width,
        self->height,
        xa > 0 ? self->width : 0,
        self->height
    );

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

    float gravity_angle = 0.0f;

    state_T* state = get_current_state();

    float gravity_force = 0.6f;
    float g_x = 0;
    float g_y = 0;
    actor_planet_T* planet = (void*)0;

    for (int i = 0; i < state->actors->size; i++)
    {
        actor_T* a = (actor_T*) state->actors->items[i];

        if (strcmp(a->type_name, "planet") == 0)
        {
            planet = (actor_planet_T*) a;
            int planet_x = a->x;
            int planet_y = a->y;
            float distance = vec2_distance(self->x, self->y, planet_x, planet_y);

            if (distance < (planet->gravity_radius * 16)/2)
            {
                gravity_angle = -vec2_angle(
                    self->x + self->width/2,
                    self->y + self->height/2,
                    planet_x,
                    planet_y
                );

                g_x = self->x + (cos(glm_rad(gravity_angle)) * (16*(planet->gravity_radius/2))) + 16/2;
                g_y = self->y - (sin(glm_rad(gravity_angle)) * (16*(planet->gravity_radius/2))) + 16/2;

                gravity_force = planet->gravity_radius - distance;
                gravity_force = gravity_force < 0 ? gravity_force*-1 : gravity_force;
                gravity_force = gravity_force * 0.003f;

                self->rz = (atan2(g_y - self->y, g_x - self->x) * 180 / M_PI) - 90.0f;

                actor_T* ground_below = get_wall_at_pos(
                    (self->x + self->width/2) + (cos(glm_rad(gravity_angle)) * (self->height/2)),
                    (self->y + self->height/2) - (sin(glm_rad(gravity_angle)) * (self->height/2)),
                    self->width,
                    self->height,
                    0,
                    0
                ); 

                if (!ground_below && planet != (void*)0) // apply gravity
                {
                    physics_vec2_push(&self->dx, &self->dy, gravity_angle, gravity_force);
                }
                else
                {
                    self->dx = 0;
                    self->dy = 0;
                }
            }
        }
    } 
}
