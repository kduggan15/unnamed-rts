#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <stdio.h>

typedef unsigned int EntityID;

// PositionComponent
typedef struct {
	EntityID entity_id;
	int x;
	int y;
	int dx;
	int dy;
} PositionComponent;

// HealthComponent
typedef struct {
	EntityID entity_id;
	int current_health;
	int max_health;
} HealthComponent;

//WeaponComponent
typedef struct {
	EntityID entity_ID;
	char name[30];
	int range;
	int damage;
} WeaponComponent;

typedef enum {
	WANDER
} AIMode;

typedef enum{
	HUMAN,
	ZOMBIE
} Faction;

typedef struct {
	EntityID entity_ID;
	AIMode mode;
	Faction fac;
} AIComponent;

#define MAX_ENTITIES 1024

//ComponentStore
typedef struct{
	PositionComponent positions[MAX_ENTITIES];
	HealthComponent healths[MAX_ENTITIES];
	WeaponComponent weapons[MAX_ENTITIES];
	AIComponent ais[MAX_ENTITIES];

	int num_positions;
	int num_healths;
	int num_weapons;
	int num_ais;

} ComponentStore;



void update_position_system(ComponentStore* store, int dt){

}
void update_health_system(ComponentStore* store){

}
void update_weapon_system(ComponentStore* store){

}

void update_ai_system(ComponentStore* store){
	
}

int update_systems(ComponentStore* store){
	update_health_system(store);
}

/*
 * Enum: CompenentMask
 * 
 * Description:
 *   Each enum represents a flag used to check if an entity has a component.
 */
typedef enum {
	POSITION = 1<<0,
	HEALTH = 1<<1,
	WEAPON = 1<<2,
	AI = 1<<3,
} ComponentMask;

/*
 * Struct: EntityManager
 * 
 * Description:
 *   EntityManager represents all the necessary fields for entity management
 * 
 * Members:
 *  component_bitmask: an array of bitmasks, for a given element indicicates what components the entity has.
 *  active: array of bools indicating if an entity is active.
 *  curEntity: an integer that keeps track of the highest 
 */
typedef struct{
	ComponentMask component_bitmask[MAX_ENTITIES];
	bool active[MAX_ENTITIES];
	EntityID cur_entity;
	EntityID free_entity_stack[MAX_ENTITIES];
	int free_entity_head;
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

int init();
int gameloop();
int cleanup();
double last_frame_time;
double last_tick_time;
ComponentStore global_store;
EntityManager global_manager;
int tick_counter;

void entity_manager_test(){
	printf("Zero entities added, %i found\n", global_manager.cur_entity);
	EntityID test1 = create_entity(&global_manager);
	printf("Adding an entity, ID is: %i\n", test1);
	EntityID test2 = create_entity(&global_manager);
	printf("Adding an entity, ID is: %i\n", test2);
	EntityID test3 = create_entity(&global_manager);
	printf("Adding an entity, ID is: %i\n", test3);

	printf("Deleting an entity, ID is: %i\n", test1);
	delete_entity(&global_manager, test1);

	EntityID test4 = create_entity(&global_manager);
	printf("Adding an entity, ID is: %i\n", test4);
	printf("ID %i and ID %i should be identical\n", test1, test4);
}

int draw(){
	char tick_str[20];
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	sprintf(tick_str, "%f", tick_counter/GetTime());
	DrawText(tick_str, 200,200,20,WHITE);
		
	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	EndDrawing();
}

int init(){
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 800, "Hello Raylib");
	SearchAndSetResourceDir("resources");
	init_entity_manager(&global_manager);
	tick_counter = 0;
	entity_manager_test();
	return 0;
}

int gameloop(){
	while (!WindowShouldClose())
	{
		// drawing
		last_frame_time = GetFrameTime();

		//Target 20 ticks per second
		if ((GetTime() - last_tick_time) > 1/20.0){
			tick_counter++;
			last_tick_time = GetTime();
			update_systems(&global_store);
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