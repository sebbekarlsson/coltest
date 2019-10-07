#include "include/actor_entity.h"


actor_entity_T* actor_entity_constructor(actor_entity_T* actor_entity)
{
    actor_entity->planet = (void*)0;
    actor_entity->gravity_angle = 0;
    actor_entity->is_on_ground = 0;
    actor_entity->should_face_gravity = 0;

    return actor_entity;
}
