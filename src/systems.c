#include "systems.h"
#include "entity.h"
#include "raylib.h"
#include <stdio.h>
void die(EntityID subject){
    delete_entity(subject);
}

void attack(EntityID subject, EntityID object){
    int damage = store.weapons[subject].damage;
    store.units[object].current_health-=damage;
}

void update_movement_system(){
	char str[20];
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(entity_is_active(e) && (entity_has_component(e,COMPONENT_MOVEMENT))){// isActive and hasComponent
            //store.positions[e].x = (int)store.movement[e].coordinates.x;
			//store.positions[e].y = (int)store.movement[e].coordinates.y;
			//Calculate velocity
			Vector2 vel;
			Vector2 dest = {store.movement[e].dest_x, store.movement[e].dest_y};//position vector
			vel = Vector2Subtract(dest, store.movement[e].coordinates);
			if(Vector2Length(vel) < store.units[e].speed/20.0+.1){
				store.movement[e].coordinates=dest;
			}
            else{
                vel = Vector2Normalize(vel);
			    vel = Vector2Scale(vel,store.units[e].speed/20.0);
			    //update coordinates
			    store.movement[e].coordinates = Vector2Add(store.movement[e].coordinates, vel);
            }
			
		
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
		
			switch (store.units[e].mode)
			{
			case AI_WANDER:
                //If you are at your destination, find a new one.
                if(pos_x==dest_x && pos_y==dest_y){
					store.movement[e].dest_x = GetRandomValue(0,512);
					store.movement[e].dest_y = GetRandomValue(0,512);
				}

                //If you have a weapon, look for enemies
                if(entity_has_component(e,COMPONENT_WEAPON)){
                    for(EntityID e0 = 0; e0<MAX_ENTITIES; e0++){
                        //Check if hostile and in sight, change to attack mode and target enemy
                        if(entity_is_active(e0) && store.units[e].fac != store.units[e0].fac && Vector2Distance(store.movement[e].coordinates, store.movement[e0].coordinates) < 15 ){
                            store.units[e].mode = AI_ATTACK;
                            store.weapons[e].target = e0;
                            break;
                        }
                    }

                }
				break;
			
            case AI_ATTACK:
                EntityID target = store.weapons[e].target;
               //Check if target still in sight
                if(!entity_is_active(target)){
                    store.units[e].mode = AI_WANDER;
                }
                    

                //If in range, attack, else move towards target
                if(Vector2Distance(store.movement[e].coordinates, store.movement[target].coordinates) < store.weapons[e].range){
                    attack(e, target);
                }else{
                    store.movement[e].dest_x = store.positions[target].x;
					store.movement[e].dest_y = store.positions[target].y;
                }
			default:
				break;
			}
		}
		//store.positions[e].dpos = dpos;
		//store.movement[e].destination = mov;
        if(store.units[e].current_health<=0){
            die(e);
        }
	}
}
void render_system(){
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	//draw all active entities
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(entity_is_active(e)){
			DrawPixel(store.positions[e].x,store.positions[e].y, store.renderables[e].color);
		}
	}
	EndDrawing();
}

int update_systems(){
	update_unit_system();
	update_movement_system();
}
