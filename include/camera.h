#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/struct.h>

#include "input.h"

typedef struct camera_{
    //camera general
    vec3s look;
    vec3s position;
    //special vector
    vec3s up;
    vec3s right;
    vec3s forward;
    //angle
    float yaw; 
    float pitch;
    //matrix
    mat4s View;
}Camera;

void initCamera(Camera *camera, vec3s position, vec3s look);

mat4s worldMatrix(mat4s View);

void matrix_init(mat4s View , unsigned int program, unsigned int *matrix, int *counter);

void cameraMovement(const Uint8 *keys, Mouse mouse, Camera *camera, Uint64 deltaTime);

#endif