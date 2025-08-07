#include "entity.h"



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
EntityID create_entity(EntityManager* manager){
	EntityID newID;
	if(manager->free_entity_head>0){
		newID = manager->free_entity_stack[manager->free_entity_head-1];
		manager->free_entity_head--;
	}else{
		newID = manager->cur_entity;
		manager->cur_entity++;
	}
	manager->component_bitmask[newID] = 0;
	manager->active[newID] = 1;
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
void delete_entity(EntityManager* manager, EntityID entity){
	manager->active[entity] = 0;
	manager->component_bitmask[entity] = 0;
	manager->free_entity_stack[manager->free_entity_head] = entity;
	manager->free_entity_head++;
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
void add_component(EntityManager* manager, EntityID entity, unsigned int component){
	manager->component_bitmask[entity] |= component;
	
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
void remove_component(EntityManager* manager, EntityID entity, unsigned int component){
	
	manager->component_bitmask[entity] &= ~component;
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
void init_entity_manager(EntityManager* manager){
	manager->cur_entity = 0;
	manager->free_entity_head = 0;
	for (int i = 0; i < MAX_ENTITIES; i++){
		manager->active[i] = 0;
		manager->component_bitmask[i] = 0;
	}
}
