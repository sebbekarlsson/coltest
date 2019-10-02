#include "include/collision.h"
#include "include/scene_world.h"
#include <coelum/current.h>


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
        self->height, xa > 0 ? self->width : 0,
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
}
