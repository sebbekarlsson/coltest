#include "include/sprites.h"
#include <coelum/textures.h>


void init_sprites()
{
    texture_T* texture_player_sheet = get_texture("res/img/M_12.png", GL_RGBA);

    SPRITE_STONE = load_sprite_from_disk("res/img/stone.spr");
    SPRITE_GRASS = load_sprite_from_disk("res/img/grass.spr");
    SPRITE_SHIP = init_sprite_from_file("res/img/Corvette.png", GL_RGBA, 0, 84, 58);
    SPRITE_BACKGROUND = init_sprite_from_file("res/img/bg_stars.png", GL_RGB, 0, 640, 480);

    dynamic_list_T* sprite_player_walk_left_textures = init_dynamic_list(sizeof(struct SPRITE_STRUCT));
    dynamic_list_append(sprite_player_walk_left_textures, get_subtexture(texture_player_sheet, 16*3, 0.0f, 16.0f, 16.0f));
    dynamic_list_append(sprite_player_walk_left_textures, get_subtexture(texture_player_sheet, 16*3, 16, 16.0f, 16.0f));
    dynamic_list_append(sprite_player_walk_left_textures, get_subtexture(texture_player_sheet, 16*3, 16*2, 16.0f, 16.0f));
    SPRITE_PLAYER_LEFT = init_sprite(sprite_player_walk_left_textures, 0.1f, 16, 16);

    dynamic_list_T* sprite_player_walk_right_textures = init_dynamic_list(sizeof(struct SPRITE_STRUCT));
    dynamic_list_append(sprite_player_walk_right_textures, get_subtexture(texture_player_sheet, 16*1, 0.0f, 16.0f, 16.0f));
    dynamic_list_append(sprite_player_walk_right_textures, get_subtexture(texture_player_sheet, 16*1, 16, 16.0f, 16.0f));
    dynamic_list_append(sprite_player_walk_right_textures, get_subtexture(texture_player_sheet, 16*1, 16*2, 16.0f, 16.0f));
    SPRITE_PLAYER_RIGHT = init_sprite(sprite_player_walk_right_textures, 0.1f, 16, 16);
}
