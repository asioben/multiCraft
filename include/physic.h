#ifndef PHYSIC_H
#define PHYSIC_H

#include "input.h"

#include <cglm/struct.h>

float *screenToWorld(vec2 mouse, vec2 screen, mat4 View, mat4 Projection);

bool raytrace(vec3 mouse, vec3 cube);

#endif