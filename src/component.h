#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity.h"
#include "raylib.h"
#include "raymath.h"

typedef enum {
	AI_WANDER
} AIMode;

typedef enum{
	FACTION_HUMAN,
	FACTION_ZOMBIE
} Faction;

/*
 * Enum: Player
 * 
 * Description:
 *   Players 1-4
 */
typedef enum{
	PLAYER_1,
	PLAYER_2,
	PLAYER_3,
	PLAYER_4,
} Player;

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
} MovementComponent;

// UnitComponent
typedef struct {
	EntityID entity_id;
	int current_health;
	int max_health;
	int speed;
	AIMode mode;
	Faction fac;
	Player player;
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
	Color color;
} RenderComponent;

typedef struct {
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
} ComponentStore;

extern ComponentStore store;

void update_position_system();
void update_movement_system();
void update_unit_system();
void update_weapon_system();
int update_systems();

#endif //COMPONENT_H