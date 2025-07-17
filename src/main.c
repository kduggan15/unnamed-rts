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

typedef struct{
	int curEntity;
} EntityManager;

int init();
int gameloop();
int cleanup();
double last_frame_time;
double last_tick_time;
ComponentStore global_store;
int tick_counter;

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
	tick_counter = 0;
	return 0;
}

int gameloop(){
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
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