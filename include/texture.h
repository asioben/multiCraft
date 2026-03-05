#ifndef TEXTURE_H
#define TEXTURE_H

#include "camera.h"

typedef enum {
    GRASS
}BlockType;

int initTexture(const char* path, unsigned int *texture);

void destroyTexture(unsigned int *texture);

void generateSprite(int ID, SDL_Point sheet, SDL_Point sprite, vec2s *uv);

void generateCube(float *vertices, BlockType block);

#endif