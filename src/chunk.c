#include "include/chunk.h"
#include <coelum/current.h>
#include <coelum/draw_utils.h>


chunk_T* init_chunk()
{
    chunk_T* chunk = calloc(1, sizeof(struct CHUNK_STRUCT));

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            chunk->walls[x][y] = init_actor_wall(0, 0, WALL_AIR);
        }
    }

    return chunk;
}

void chunk_tick(chunk_T* self)
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            actor_T* actor = (actor_T*) self->walls[x][y];
            actor->tick(actor);
        }
    }
}

void chunk_draw(chunk_T* self, int x, int y)
{
    state_T* state = get_current_state();

    camera_bind(state->camera);

    for (int xx = 0; xx < 8; xx++)
    {
        for (int yy = 0; yy < 8; yy++)
        {
            actor_T* actor = (actor_T*) self->walls[xx][yy];
            actor->x = x+(xx*16);
            actor->y = y+(yy*16);
            actor_draw_default(actor, state);
        }
    }

    /*draw_line(
        x,
        y,
        0.0f,
        x,
        16*8,
        0,
        255,
        0,
        0,
        state
    );

    draw_line(
        x,
        y,
        0.0f,
        16*8,
        y,
        0,
        255,
        0,
        0,
        state
    );*/

    camera_unbind(state->camera);
}
