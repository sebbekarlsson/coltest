#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/scene.h>
#include <coelum/utils.h>
#include <string.h>
#include "include/sprites.h"
#include "include/scene_world.h"


extern theatre_T* THEATRE;

#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif


void scene_main_tick(scene_T* scene)
{
    state_T* state = (state_T*) scene;

    float x = 0;
    float y = 0;

    for (int i = 0; i < state->actors->size; i++)
    {
        actor_T* actor = (actor_T*) state->actors->items[i];

        if (strcmp(actor->type_name, "player") == 0)
        {
            x = actor->x;
            y = actor->y;
            break;
        }
    }

    state->camera->x = x - (640 /2);
    state->camera->y = y - (480 / 2);
}

/*scene_T* init_scene_main()
{
    init_random();

    // creating a scene                          tick        draw     (2 dimensions)
    scene_T* s = scene_constructor(init_scene(), scene_main_tick, (void*) 0, 2);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;


    float p_x = 0;
    float p_y = 0;
    float planet_r = 16;
    float planet_x = p_x;
    float planet_y = p_y + (((planet_r*16) / 2) + 16);

    dynamic_list_append(((state_T*)s)->actors, init_actor_player(p_x, p_y));

    world_generate_planet((state_T*)s, planet_r, planet_x, planet_y);

    return s;
}*/

int main(int argc, char* argv[])
{
    coelum_init();
    init_sprites();

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_world());

    return coelum_main(argc, argv);
}
