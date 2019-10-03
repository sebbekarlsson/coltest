#include "include/actor_planet.h"
#include <coelum/utils.h>
#include <coelum/current.h>


actor_planet_T* init_actor_planet(float x, float y, scene_world_T* world, int mass_radius, int gravity_radius)
{
    actor_planet_T* planet = calloc(1, sizeof(struct ACTOR_PLANET_STRUCT));
    actor_T* actor = (actor_T*) planet;

    actor_constructor(actor, x, y, 0, actor_planet_tick, actor_planet_draw, "planet");

    planet->world = world;
    planet->mass_radius = mass_radius;
    planet->gravity_radius = gravity_radius;
    planet->is_generated = 0;

    return planet;
}

void actor_planet_generate(actor_planet_T* self)
{
    actor_T* actor = (actor_T*) self;

    scene_world_T* world = self->world;

    int x = (int) actor->x;
    int y = (int) actor->y;
    int r = self->mass_radius;

    x = x / 16;
    y = y / 16;

    for (int i = x - (r / 2); i < x + (r / 2); i++)
    {
        for (int j = y - (r / 2); j < y + (r / 2); j++)
        {
            chunk_T* chunk = scene_world_get_chunk_at(world, i * 16, j * 16);

            float distance = vec2_distance(i, j, x, y);

            int wall_x = (i) % 8;
            int wall_y = (j) % 8;

            actor_wall_T* wall = chunk->walls[wall_x][wall_y];

            if (distance < r / 2)
            {
                wall->type = distance > (((r / 2)-random_int(1, 3))) ? WALL_GRASS : WALL_STONE;
                actor_wall_update(wall);
            }
        }
    }
}

void actor_planet_tick(actor_T* self) {}

void actor_planet_draw(actor_T* self) {}
