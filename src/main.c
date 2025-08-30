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



void entity_manager_test(){
	#define UNITS 10
	EntityID es[UNITS];
	for (int i=0; i<UNITS; i++){
		if(i%2==0){
			es[i] = create_infected(PLAYER_1, i*5, i*5);
		}
		else{
			es[i] = create_civilian(PLAYER_1, GetRandomValue(0,100), GetRandomValue(0,100));
		}
		
		store.units[es[i]].mode = AI_WANDER;
	}
	
}

int init(){
	SetRandomSeed(15);
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(SCREEN_SIZE_X, SCREEN_SIZE_Y, "Hello Raylib");
	SearchAndSetResourceDir("resources");
	init_entity_manager();
	tick_counter = 0;
	entity_manager_test();
	init_render_system();
	return 0;
}

int gameloop(){
	
	while (!WindowShouldClose())
	{
		
		control_system();
		// drawing
		last_frame_time = GetFrameTime();

		//Target 20 ticks per second
		if ((GetTime() - last_tick_time) > 1/20.0){
			tick_counter++;
			last_tick_time = GetTime();
			update_systems();
		}
		render_system();
		
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