#include "entity.h"

static unsigned int component_bitmask[MAX_ENTITIES];
static bool active[MAX_ENTITIES];
static EntityID cur_entity;
static EntityID free_entity_stack[MAX_ENTITIES];
static int free_entity_head;

/*
 * Name: create_Entity
 *
 * Description: 
 *   Creates an entity and returns its ID. Will use a recycled entityID first.
 *
 * Parameters:
 *   manager: EntityManager struct that will manage the created Entity.
 * 
 * Returns: 
 *   Returns an EntityID for a newly created entity
 * 
 * Side Effects:
 *   manager will track the new EntityID
 */
EntityID create_entity(){
	EntityID newID;
	if(  free_entity_head>0){
		newID = free_entity_stack[free_entity_head-1];
		free_entity_head--;
	}else{
		newID = cur_entity;
		cur_entity++;
	}
	  component_bitmask[newID] = 0;
	  active[newID] = 1;
	return newID;
}

/*
 * Name: delete_Entity
 *
 * Description: 
 *   Removes an entity if it exists and frees the ID from the manager
 *
 * Parameters:
 *   manager: EntityManager struct that entity belongs to.
 * 
 * Returns: 
 *   void
 * 
 * Side Effects:
 *   manager will have the entity removed.
 */
void delete_entity(EntityID entity){
	active[entity] = 0;
	component_bitmask[entity] = 0;
	free_entity_stack[free_entity_head] = entity;
	free_entity_head++;
}

/*
 * Name: add_component
 *
 * Description: 
 *   Adds a component to an entity on the entity_manager
 *
 * Parameters:
 *   manager: EntityManager struct that entity belongs to.
 *   entity: EntityID of the entity that will have the component added
 *   component: Component enum to be added to the entity
 * 
 * Returns: 
 *   void
 * 
 * Side Effects:
 *   manager will have the entity updated.
 */
void add_component(EntityID entity, unsigned int component){
	component_bitmask[entity] |= component;
}

/*
 * Name: remove_component
 *
 * Description: 
 *   removes a component to an entity on the entity_manager
 *
 * Parameters:
 *   manager: EntityManager struct that entity belongs to.
 *   entity: EntityID of the entity that will have the component added
 *   component: Component enum to be removed from the entity
 * 
 * Returns: 
 *   void
 * 
 * Side Effects:
 *   manager will have the entity updated.
 */
void remove_component(EntityID entity, unsigned int component){
	
	component_bitmask[entity] &= ~component;
}
/*
 * Name: init_entity_manager
 *
 * Description: 
 *   Creates an entity and returns its ID.
 *
 * Parameters:
 *   manager: EntityManager struct that will manage the created Entity.
 * 
 * Returns: 
 *   Returns an EntityID for a newly created entity
 * 
 * Side Effects:
 *   manager will track the new EntityID
 */
void init_entity_manager(){
	cur_entity = 0;
	free_entity_head = 0;
	for (int i = 0; i < MAX_ENTITIES; i++){
		active[i] = 0;
		component_bitmask[i] = 0;
	}
}

bool entity_is_active(EntityID e){
    return active[e];
}

bool entity_has_component(EntityID e, unsigned int component){
    return component_bitmask[e] & component;
}