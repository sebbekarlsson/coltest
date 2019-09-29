#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/scene.h>
#include <coelum/utils.h>
#include "include/actor_wall.h"
#include "include/actor_player.h"


extern theatre_T* THEATRE;

#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

scene_T* init_scene_main()
{
    init_random();

    // creating a scene                          tick        draw     (2 dimensions)
    scene_T* s = scene_constructor(init_scene(), (void*) 0, (void*) 0, 2);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;
    
    dynamic_list_append(((state_T*)s)->actors, init_actor_player(200, 120));

    int nr_planets = 5;

    for (int i = 0; i < nr_planets; i++)
    {
        int r = random_int(3, 9)*16;
        int xx = random_int(0, 640);
        int yy = random_int(0, 480);

        for (int x = xx-(r/2); x < xx+(r/2); x+=16)
        {
            for (int y = yy-(r/2); y < yy+(r/2); y+=16)
            {
                if (vec2_distance(x+(16/2), y+(16/2), xx, yy) < r / 2)
                    dynamic_list_append(((state_T*)s)->actors, init_actor_wall(x, y));
            }
        }
    }

    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
}
