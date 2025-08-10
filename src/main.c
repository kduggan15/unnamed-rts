#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"
#include "entity.h"
#include "component.h"
#include "systems.h"


int init();
int gameloop();
int cleanup();
double last_frame_time;
double last_tick_time;
int tick_counter;

EntityID create_infected(Player player, int x, int y){
	EntityID e = create_entity();
	add_component(e,COMPONENT_UNIT|COMPONENT_POSITION|COMPONENT_MOVEMENT|COMPONENT_WEAPON|COMPONENT_RENDER);
	entity_is_active(e);

	store.positions[e].x = store.movement[e].coordinates.x = store.movement[e].dest_x = x;
	store.positions[e].y = store.movement[e].coordinates.y = store.movement[e].dest_y = y;

	store.units[e].max_health = 100;
	store.units[e].current_health=100;
	store.units[e].fac = FACTION_ZOMBIE;
	store.units[e].type = UNIT_INFECTED;
	store.units[e].speed = 3;
	store.units[e].player = player;
	store.weapons[e].damage = 10;
	
	strcpy(store.weapons[e].name, "Claws");
	store.renderables[e].color=RED;

	return e;
}

EntityID create_civilian(Player player, int x, int y){
	EntityID e = create_entity();
	add_component(e,COMPONENT_UNIT|COMPONENT_POSITION|COMPONENT_MOVEMENT|COMPONENT_RENDER);
	entity_is_active(e);

	store.positions[e].x = store.movement[e].coordinates.x = store.movement[e].dest_x = x;
	store.positions[e].y = store.movement[e].coordinates.y = store.movement[e].dest_y = y;

	store.units[e].max_health = 50;
	store.units[e].current_health=50;
	store.units[e].fac = FACTION_HUMAN;
	store.units[e].type = UNIT_CIVILIAN;
	store.units[e].speed = 2;
	store.units[e].player = player;
	store.renderables[e].color=GRAY;

	return e;
}

void entity_manager_test(){
	#define UNITS 10
	EntityID es[UNITS];
	for (int i=0; i<UNITS; i++){
		if(i%2==0){
			es[i] = create_infected(PLAYER_1, i*5, i*5);
		}
		else{
			es[i] = create_civilian(PLAYER_1, i*5, i*5);
		}
		
		store.units[es[i]].mode = AI_WANDER;
	}
	
}

int draw(){
	char tick_str[20];
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	//draw all active entities
	for(EntityID e = 0; e<MAX_ENTITIES;e++){
		if(entity_is_active(e)){
			//sprintf(tick_str, "%i", e);
			//printf("entity 0 pos: %i, %i\n", global_manager.positions[0].x,global_manager.positions[0].y);
			//DrawText(tick_str, global_manager.positions[e].x,global_manager.positions[e].y,20,WHITE);
			DrawPixel(store.positions[e].x,store.positions[e].y, store.renderables[e].color);
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
	init_entity_manager();
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
			update_systems();
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