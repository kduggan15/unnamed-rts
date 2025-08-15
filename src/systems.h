#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "component.h"

void update_movement_system();
void update_unit_system();
void render_system();
int update_systems();

extern Camera2D camera;

#endif // SYSTEMS_H