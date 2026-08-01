#ifndef SCENE_H
#define SCENE_H

#include "graphic.h"
#include "block.h"
//#include "camera.h"

typedef enum{
    TITLE,
    GAME
}Scene;

void sceneManagement(Scene *scene, Mesh *meshes, unsigned int program, unsigned int texture, unsigned int VAO);

int initTitle(unsigned int *VAO, unsigned int *VBO, unsigned int *texture);

//Render the game scene (in case)
void render(Mesh *meshes, int size, unsigned int program, unsigned int texture);

#endif