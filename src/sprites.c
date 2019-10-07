#include "include/sprites.h"


void init_sprites()
{
    SPRITE_STONE = load_sprite_from_disk("res/img/stone.spr");
    SPRITE_GRASS = load_sprite_from_disk("res/img/grass.spr");
    
    SPRITE_PLAYER_LEFT = load_sprite_from_disk("res/img/player_left.spr");
    
    SPRITE_BACKGROUND = init_sprite_from_file("res/img/bg_stars.png", GL_RGB, 0, 640, 480);
}
