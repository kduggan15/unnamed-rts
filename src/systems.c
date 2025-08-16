#include "systems.h"
#include "entity.h"
#include "raylib.h"
#include <stdio.h>

Camera2D camera = {0};

void die(EntityID subject){
    delete_entity(subject);
}

void attack(EntityID subject, EntityID object){
    int damage = store.weapons[subject].damage;
    store.units[object].current_health-=damage;
}

void update_movement_system(){
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(entity_is_active(e) && (entity_has_component(e,COMPONENT_MOVEMENT))){// isActive and hasComponent
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

            //update position. Whenever coordinates are updated, position must be updated.
            store.positions[e].x = (int)store.movement[e].coordinates.x;
			store.positions[e].y = (int)store.movement[e].coordinates.y;
        }
	}
}

/*
 * Name: find_target
 *
 * Description: 
 *   Looks for an entity in the opposite faction that is within site and assigns it as the target
 *
 * Parameters:
 *   e: EntityID, the entity looking for a target. MUST HAVE A WEAPON, MUST BE A UNIT
 * 
 * Returns: 
 *   NA
 * 
 * Side Effects:
 *   If a target is found, the weapon's target is set to the found entity. and the unit is put in attack mode
 */
void find_target(EntityID e){
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
					store.movement[e].dest_x = GetRandomValue(0,MAP_SIZE_X);
					store.movement[e].dest_y = GetRandomValue(0,MAP_SIZE_Y);
				}

                //If you have a weapon, look for enemies
                if(entity_has_component(e,COMPONENT_WEAPON)){
					find_target(e);
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
        if(store.units[e].current_health<=0){
            die(e);
        }
	}
}

void control_system(){
	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)){
		camera.target = Vector2Subtract(camera.target, GetMouseDelta());
		if(camera.target.x<0)
			camera.target.x=0;
		if(camera.target.y<0)
			camera.target.y=0;
		if(camera.target.x>MAP_SIZE_X)
			camera.target.x=MAP_SIZE_X;
		if(camera.target.y>MAP_SIZE_Y)
			camera.target.y=MAP_SIZE_Y;
		}
		camera.zoom = camera.zoom += GetMouseWheelMove() * 0.5f;
		if(camera.zoom <3.0f) camera.zoom = 3.0f;
		if(camera.zoom >10.0f) camera.zoom = 10.0f;
}

void init_render_system(){
	camera.target.x=10;
	camera.target.y=10;
	camera.offset.x = SCREEN_SIZE_X/2.0f;
	camera.offset.y = SCREEN_SIZE_Y/2.0f;
	camera.rotation = 0;
	camera.zoom = 7;
}

void render_entity(EntityID e){
	DrawCircle(store.movement[e].coordinates.x,store.movement[e].coordinates.y,1.0,store.renderables[e].color);
}

void render_system(){
	BeginDrawing();
	ClearBackground(BLACK);

	BeginMode2D(camera);

	//draw map bounds
	DrawRectangleLines(0,0,MAP_SIZE_X,MAP_SIZE_Y,WHITE);

	//draw all active entities
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(entity_is_active(e) && entity_has_component(e,COMPONENT_RENDER)){
			render_entity(e);
		}
	}

	EndMode2D();

	EndDrawing();
}

int update_systems(){
	update_unit_system();
	update_movement_system();
}
