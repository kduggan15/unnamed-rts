#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#define MAX_ENTITIES 1024
typedef unsigned int EntityID;

EntityID create_entity();
void delete_entity(EntityID entity);
void add_component(EntityID entity, unsigned int component);
void remove_component(EntityID entity, unsigned int component);
void init_entity_manager();
bool entity_is_active(EntityID);
bool entity_has_component(EntityID entity, unsigned int component);

#endif// ENTITY_H