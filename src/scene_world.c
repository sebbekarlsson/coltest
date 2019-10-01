#include "include/scene_world.h"
#include <coelum/utils.h>


scene_world_T* init_scene_world()
{
    scene_world_T* world = calloc(1, sizeof(struct SCENE_WORLD_STRUCT));
    scene_T* scene = (scene_T*) world;

    scene_constructor(scene, scene_world_tick, scene_world_draw, 2);

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            world->chunks[x][y] = init_chunk();
        }
    }

    scene_world_generate(world);

    return world;
}

void scene_world_tick(scene_T* self)
{
    scene_world_T* world = (scene_world_T*) self;

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            chunk_tick(world->chunks[x][y]);
        }
    }
}

void scene_world_draw(scene_T* self)
{
    scene_world_T* world = (scene_world_T*) self;

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            chunk_draw(world->chunks[x][y], x*(16*8), y*(16*8));
        }
    }
}

static void generate_planet(scene_world_T* world, int r, int x, int y)
{
    for (int i = x-(r/2); i < x+(r/2); i++)
    {
        for (int j = y-(r/2); j < y+(r/2); j++)
        {
            int chunk_y = 0;
            int chunk_x = 0;

            chunk_x = (i/16)%16;
            chunk_y = (j/16)%16;

            printf("chx %d, chy %d\n", chunk_x, chunk_y);

            chunk_T* chunk = world->chunks[chunk_x][chunk_y];

            float distance = vec2_distance(i, j, x, y);

            int wall_x = i % 8;
            int wall_y = j % 8;

            actor_wall_T* wall = chunk->walls[wall_x][wall_y];

            printf("CHUNK(%d, %d)->WALL(%d, %d)\n", chunk_x, chunk_y, wall_x, wall_y);

            printf("DISTANCE: %12.6f, RADIUS: %d\n", distance, r);

            if (distance < r/2)
            {
                wall->type = distance > (((r/2)-random_int(1, 3))) ? WALL_GRASS : WALL_STONE;
                actor_wall_update(wall);
            }
        }
    }
}

void scene_world_generate(scene_world_T* self)
{
    generate_planet(self, 16, 24, 24);
}
