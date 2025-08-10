#include "systems.h"
#include "entity.h"
#define MAP_SIZE_X 512
#define MAP_SIZE_Y 512


void update_movement_system(){
	char str[20];
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(entity_is_active(e) && (entity_has_component(e,COMPONENT_MOVEMENT))){// isActive and hasComponent
			//Calculate velocity
			Vector2 vel;
			Vector2 dest = {store.movement[e].dest_x, store.movement[e].dest_y};//position vector
			vel = Vector2Subtract(dest, store.movement[e].coordinates);
			if(Vector2Length(vel) < store.units[e].speed+.2){
				store.movement[e].coordinates=dest;
				break;
			}
			vel = Vector2Normalize(vel);
			vel = Vector2Scale(vel,store.units[e].speed/20.0);
			//update coordinates
			store.movement[e].coordinates = Vector2Add(store.movement[e].coordinates, vel);
		
            //update position
            store.positions[e].x = (int)store.movement[e].coordinates.x;
			store.positions[e].y = (int)store.movement[e].coordinates.y;
        }
	}
}
void update_unit_system(){
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		char str[20];
		int pos_x;
		int pos_y;
		int dest_x;
		int dest_y;
		//Vector2 velocity;
        if(entity_is_active(e) && (entity_has_component(e,COMPONENT_UNIT))){//True if entity is active, and entity has AI
		    pos_x = store.positions[e].x;
		    pos_y = store.positions[e].y;
		    dest_x = store.movement[e].dest_x;
		    dest_y = store.movement[e].dest_y;
		
			switch (store.units->mode)
			{
			case AI_WANDER:

				if(pos_x==dest_x && pos_y==dest_y){
					store.movement[e].dest_x = GetRandomValue(0,512);
					store.movement[e].dest_y = GetRandomValue(0,512);
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

int update_systems(){
	update_unit_system();
	update_movement_system();
}
