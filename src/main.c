#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/scene.h>
#include "include/actor_wall.h"
#include "include/actor_player.h"


extern theatre_T* THEATRE;

#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

scene_T* init_scene_main()
{
    // creating a scene                          tick        draw     (2 dimensions)
    scene_T* s = scene_constructor(init_scene(), (void*) 0, (void*) 0, 2);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;
    
    dynamic_list_append(((state_T*)s)->actors, init_actor_player(200, 120));

    for (int i = 0; i < 10; i++)
        dynamic_list_append(((state_T*)s)->actors, init_actor_wall(120 + (i * 16), 300));

    for (int i = 0; i < 6; i++)
        dynamic_list_append(((state_T*)s)->actors, init_actor_wall(256 + (i * 16), 348));

    for (int i = 0; i < 10; i++)
        dynamic_list_append(((state_T*)s)->actors, init_actor_wall(300 + (i * 16), 400));

    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
}
