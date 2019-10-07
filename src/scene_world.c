#include "include/scene_world.h"
#include "include/actor_player.h"
#include "include/actor_ship.h"
#include "include/actor_planet.h"
#include <coelum/utils.h>
#include <coelum/draw_utils.h>
#include <string.h>


extern sprite_T* SPRITE_BACKGROUND;

scene_world_T* init_scene_world()
{
    scene_world_T* world = calloc(1, sizeof(struct SCENE_WORLD_STRUCT));
    scene_T* scene = (scene_T*) world; 

    scene_constructor(scene, scene_world_tick, scene_world_draw, 2);

    scene->bg_r = 0;
    scene->bg_g = 0;
    scene->bg_b = 0;

    for (int x = 0; x < NR_CHUNKS; x++)
    {
        for (int y = 0; y < NR_CHUNKS; y++)
        {
            world->chunks[x][y] = init_chunk();
        }
    }
    
    dynamic_list_append(((state_T*)scene)->actors, (actor_T*)init_actor_player(
                640/2, (480/2)-((16*18)/2)));

    actor_ship_T* ship = init_actor_ship(
                640/2, (480/2)+((16*24)/2));
    ((actor_T*)ship)->rz += 180.0f;

    dynamic_list_append(((state_T*)scene)->actors, (actor_T*)ship);

    scene_world_generate(world);

    return world;
}

void scene_world_tick(scene_T* self)
{
    scene_world_T* world = (scene_world_T*) self;
    state_T* state = (state_T*) self;

    for (int x = 0; x < NR_CHUNKS; x++)
    {
        for (int y = 0; y < NR_CHUNKS; y++)
        {
            chunk_tick(world->chunks[x][y]);
        }
    }

    float cam_x = 0;
    float cam_y = 0;
    float rz = 0;

    for (int i = 0; i < state->actors->size; i++)
    {
        actor_T* actor = (actor_T*) state->actors->items[i];

        if (strcmp(actor->type_name, "player") == 0)
        {
            cam_x = actor->x;
            cam_y = actor->y;
            rz = actor->rz;
            break;
        }
    }

    state->camera->x = (cam_x-640/2);
    state->camera->y = (cam_y-480/2);
    state->camera->offset_x = cam_x;
    state->camera->offset_y = cam_y;
    float rot_vel = 0.5f;

    if (state->camera->rz < rz)
    {
        if (state->camera->rz + rot_vel > rz)
        {
            state->camera->rz = rz;
        }
        else
        {
            state->camera->rz += rot_vel;
        }
    }
    else
    if (state->camera->rz > rz)
    {
        if (state->camera->rz - rot_vel < rz)
        {
            state->camera->rz = rz;
        }
        else
        {
            state->camera->rz -= rot_vel;
        }
    }
}

void scene_world_draw(scene_T* self)
{
    scene_world_T* world = (scene_world_T*) self;
    state_T* state = (state_T*) self;
    camera_T* camera = state->camera; 

    for (int x = 0; x < NR_CHUNKS; x++)
    {
        for (int y = 0; y < NR_CHUNKS; y++)
        {
            int size = (16*8);
            int chx = x*size;
            int chy = y*size;

            if (chx+size < camera->x || (chx-size) > (camera->x + 640))
                continue;

            if (chy+size < camera->y || (chy-size) > (camera->y + 480))
                continue;
             
            chunk_draw(world->chunks[x][y], chx, chy);
        }
    }

    draw_positioned_sprite(
        SPRITE_BACKGROUND,
        0, 0, 0,
        640, 480,
        state        
    );
}

chunk_T* scene_world_get_chunk_at(scene_world_T* world, int x, int y)
{
    return world->chunks[x / (16 * 8)][y / (16 * 8)];
}

void scene_world_generate(scene_world_T* self)
{
    state_T* state = (state_T*) self;

    actor_planet_T* planet = init_actor_planet(640/2, 480/2, self, 17, 17+8);
    actor_planet_generate(planet);
    dynamic_list_append(state->actors, (actor_T*) planet);

    actor_planet_T* planet2 = init_actor_planet((640/2) + (30*16), (480/2), self, 17, 17+8);
    actor_planet_generate(planet2);
    dynamic_list_append(state->actors, (actor_T*) planet2);

    actor_planet_T* planet3 = init_actor_planet((640/2) + (30*16), (480/2) + (30*16), self, 17, 17+8);
    actor_planet_generate(planet3);
    dynamic_list_append(state->actors, (actor_T*) planet3);
}
