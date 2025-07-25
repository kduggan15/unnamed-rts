#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <stdio.h>

#define MAX_ENTITIES 1024

typedef unsigned int EntityID;

typedef enum {
	AI_WANDER
} AIMode;

typedef enum{
	FACTION_HUMAN,
	FACTION_ZOMBIE
} Faction;

/*
 * Enum: CompenentMask
 * 
 * Description:
 *   Each enum represents a flag used to check if an entity has a component.
 */
typedef enum {
	COMPONENT_POSITION = 1<<0,
	COMPONENT_MOVEMENT = 1<<1,
	COMPONENT_UNIT = 1<<2,
	COMPONENT_WEAPON = 1<<3,
	COMPONENT_RENDER = 1<<4
} ComponentMask;

// PositionComponent
typedef struct {
	EntityID entity_id;
	int x;
	int y;
} PositionComponent;

// MovementComponent
// Movement is accomplished by having a final location and moving towards it each update.
typedef struct {
	EntityID entity_id;
	int dest_x;
	int dest_y;
	Vector2 velocity;
	Vector2 coordinates;//float equivalent of position for entities that can move
	float progress; // For cases where multiple ticks are required to move a single tile. 1.0 represents a complete move.
} MovementComponent;

// UnitComponent
typedef struct {
	EntityID entity_id;
	int current_health;
	int max_health;
	int speed;
	AIMode mode;
	Faction fac;
} UnitComponent;

//WeaponComponent
typedef struct {
	EntityID entity_ID;
	char name[30];
	int range;
	int damage;
} WeaponComponent;

typedef struct {
	EntityID entity_ID;
} RenderComponent;

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
typedef struct{
	ComponentMask component_bitmask[MAX_ENTITIES];
	bool active[MAX_ENTITIES];
	EntityID cur_entity;
	EntityID free_entity_stack[MAX_ENTITIES];
	int free_entity_head;

	// Component portion
	PositionComponent positions[MAX_ENTITIES];
	MovementComponent movement[MAX_ENTITIES];
	UnitComponent units[MAX_ENTITIES];
	WeaponComponent weapons[MAX_ENTITIES];
	RenderComponent renderables[MAX_ENTITIES];

	int num_positions;
	int num_movement;
	int num_units;
	int num_weapons;
	int num_renderables;
} EntityManager;

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
void add_component(EntityManager* manager, EntityID entity, ComponentMask component){
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
void remove_component(EntityManager* manager, EntityID entity, ComponentMask component){
	
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

void update_position_system(EntityManager* manager){
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_POSITION)){
			//if it has a movement component, update the position
			if(manager->component_bitmask[e] & COMPONENT_MOVEMENT){
				manager->positions[e].x = (int)manager->movement[e].coordinates.x;
				manager->positions[e].y = (int)manager->movement[e].coordinates.y;
				printf("Update positions\n");
			}
		}
	}
}
void update_movement_system(EntityManager* manager){
	char str[20];
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_MOVEMENT)){
			//Calculate velocity
			Vector2 vel;
			Vector2 dest = {manager->movement[e].dest_x, manager->movement[e].dest_y};//position vector
			
			sprintf(str, "%i", e);
			printf("Update movement\n");
			vel = Vector2Subtract(dest, manager->movement[e].coordinates);
			if(Vector2Length(vel)<manager->units[e].speed+.1){
				manager->movement[e].coordinates=dest;
			}
			vel = Vector2Normalize(vel);
			vel = Vector2Scale(vel,manager->units[e].speed);
			printf("%f,%f\n towards %f, %f",vel.x,vel.y, dest.x,dest.y);
			//update coordinates
			
			manager->movement[e].coordinates = Vector2Add(manager->movement[e].coordinates, vel);
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
		pos_x = manager->positions[e].x;
		pos_y = manager->positions[e].y;
		dest_x = manager->movement[e].dest_x;
		dest_y = manager->movement[e].dest_y;
		if(manager->active[e] && (manager->component_bitmask[e] & COMPONENT_UNIT)){//True if entity is active, and entity has AI
			switch (manager->units->mode)
			{
			case AI_WANDER:
				
				if(pos_x==dest_x && pos_y==dest_y){
					//sprintf(str, "%i", e);
					printf("New wander\n");
					manager->movement[e].dest_x = GetRandomValue(0,1280);
					manager->movement[e].dest_y = GetRandomValue(0,800);
				}
				break;
			
			default:
				break;
			}
		}
		//manager->positions[e].dpos = dpos;
		//manager->movement[e].destination = mov;
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

int init();
int gameloop();
int cleanup();
double last_frame_time;
double last_tick_time;
EntityManager global_manager;
int tick_counter;

void entity_manager_test(){
	printf("Zero entities added, %i found\n", global_manager.cur_entity);
	EntityID test1 = create_entity(&global_manager);
	printf("Adding an entity, ID is: %i\n", test1);
	EntityID test2 = create_entity(&global_manager);
	printf("Adding an entity, ID is: %i\n", test2);
	
	add_component(&global_manager,test1,COMPONENT_UNIT|COMPONENT_POSITION|COMPONENT_MOVEMENT);
	add_component(&global_manager,test2,COMPONENT_UNIT|COMPONENT_POSITION|COMPONENT_MOVEMENT);

	global_manager.units[test1].speed=1;
	global_manager.units[test2].speed=3;

	global_manager.units[test1].mode=AI_WANDER;
	global_manager.units[test2].mode=AI_WANDER;
}

int draw(){
	char tick_str[20];
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	sprintf(tick_str, "%f", tick_counter/GetTime());
	DrawText(tick_str, 200,200,20,WHITE);

	//draw all active entities
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(global_manager.active[e]){
			sprintf(tick_str, "%i", e);
			//printf("entity 0 pos: %i, %i\n", global_manager.positions[0].x,global_manager.positions[0].y);
			DrawText(tick_str, global_manager.positions[e].x,global_manager.positions[e].y,20,WHITE);
			DrawText(tick_str, 10,10,20,WHITE);
		}
	}
		
	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	EndDrawing();
}

int init(){
	// Tell the window to use vsync and work on high DPI displays
	SetRandomSeed(15);
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 800, "Hello Raylib");
	SearchAndSetResourceDir("resources");
	init_entity_manager(&global_manager);
	tick_counter = 0;
	entity_manager_test();
	return 0;
}

int gameloop(){
	char str[20];
	while (!WindowShouldClose())
	{
		// drawing
		last_frame_time = GetFrameTime();

		//Target 20 ticks per second
		if ((GetTime() - last_tick_time) > 1/20.0){
			tick_counter++;
			last_tick_time = GetTime();
			update_systems(&global_manager);
			//sprintf(str,"%i",global_manager.positions[0].x);
			//printf(str);
		}
		draw();
		
	}

	return 0;
}

// cleanup
int cleanup(){
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
}

int main ()
{
	int status = 0;
	status ^= init();
	status ^= gameloop();
	status ^= cleanup();
	return status;
}