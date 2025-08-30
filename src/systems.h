#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "component.h"

void update_movement_system();
void update_unit_system();
void render_system();
void control_system();
void init_render_system();
int update_systems();
EntityID create_infected(Player player, int x, int y);
EntityID create_civilian(Player player, int x, int y);

extern Camera2D camera;

#endif // SYSTEMS_H