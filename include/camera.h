#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/struct.h>

#include "input.h"

mat4s getCamera(vec3s position, vec3s look);

mat4s worldMatrix(mat4s View);

void matrix_init(mat4 World, unsigned int program, unsigned int *matrix, int *counter);

//void camera_movement(vec3s *position, vec3s *look, const Uint8 *keys, Mouse mouse);

#endif