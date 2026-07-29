#ifndef PHYSIC_H
#define PHYSIC_H

#include "input.h"

#include <cglm/struct.h>

#define SQUARE(x) ((x) * (x))

void screenToWorld(vec2 mouse, vec2 screen, mat4 View, mat4 Projection, float *ray);

bool raytrace(vec3 mouse, vec3 ray, vec3 cube);

void accelerate(vec3 acceleration, float *speed, float *position, float time);

#endif