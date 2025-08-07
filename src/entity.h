#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#define MAX_ENTITIES 1024
typedef unsigned int EntityID;

/*
 * Struct: EntityManager
 * 
 * Description:
 *   EntityManager represents all the necessary fields for entity-component
 * 
 * Members:
 *  component_bitmask: an array of bitmasks, for a given element indicicates what components the entity has.
 *  active: array of bools indicating if an entity is active.
 *  cur_entity: an integer that keeps track of the highest 
 *  free_entity_stack: a stack of entity ID's that can be re-used.
 */
typedef struct EntityManager{
	unsigned int component_bitmask[MAX_ENTITIES];
	bool active[MAX_ENTITIES];
	EntityID cur_entity;
	EntityID free_entity_stack[MAX_ENTITIES];
	int free_entity_head;
} EntityManager;

EntityID create_entity(EntityManager* manager);
void delete_entity(EntityManager* manager, EntityID entity);
void add_component(EntityManager* manager, EntityID entity, unsigned int component);
void remove_component(EntityManager* manager, EntityID entity, unsigned int component);
void init_entity_manager(EntityManager* manager);

#endif// ENTITY_H