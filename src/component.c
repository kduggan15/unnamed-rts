#include "component.h"

ComponentStore store;

void update_position_system(EntityManager* manager){
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_POSITION)){
			//if it has a movement component, update the position
			if(manager->component_bitmask[e] & COMPONENT_MOVEMENT){
				store.positions[e].x = (int)store.movement[e].coordinates.x;
				store.positions[e].y = (int)store.movement[e].coordinates.y;
			}
		}
	}
}
void update_movement_system(EntityManager* manager){
	char str[20];
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_MOVEMENT)){// isActive and hasComponent
			//Calculate velocity
			Vector2 vel;
			Vector2 dest = {store.movement[e].dest_x, store.movement[e].dest_y};//position vector
			vel = Vector2Subtract(dest, store.movement[e].coordinates);
			if(Vector2Length(vel) < store.units[e].speed+.2){
				store.movement[e].coordinates=dest;
				break;
			}
			vel = Vector2Normalize(vel);
			vel = Vector2Scale(vel,store.units[e].speed);
			//update coordinates
			
			store.movement[e].coordinates = Vector2Add(store.movement[e].coordinates, vel);
		}
	}
}
void update_unit_system(EntityManager* manager){
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		char str[20];
		int pos_x;
		int pos_y;
		int dest_x;
		int dest_y;
		//Vector2 velocity;
		pos_x = store.positions[e].x;
		pos_y = store.positions[e].y;
		dest_x = store.movement[e].dest_x;
		dest_y = store.movement[e].dest_y;
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_UNIT)){//True if entity is active, and entity has AI
			switch (store.units->mode)
			{
			case AI_WANDER:
				
				if(pos_x==dest_x && pos_y==dest_y){
					store.movement[e].dest_x = GetRandomValue(0,800);
					store.movement[e].dest_y = GetRandomValue(0,800);
				}
				break;
			
			default:
				break;
			}
		}
		//store.positions[e].dpos = dpos;
		//store.movement[e].destination = mov;
	}
}
void update_weapon_system(EntityManager* manager){
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_WEAPON)){
		}
	}
}

int update_systems(EntityManager* manager){
	update_unit_system(manager);
	update_movement_system(manager);
	update_position_system(manager);
}