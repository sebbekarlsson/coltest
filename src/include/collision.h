#ifndef COLLISION_H
#define COLLISION_H
#include <coelum/actor.h>


actor_T* get_wall_at_pos(float x, float y, float w, float h, int x_offset, int y_offset);

void move(actor_T* self, float xa, float ya);
#endif
