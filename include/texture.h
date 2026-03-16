#ifndef TEXTURE_H
#define TEXTURE_H

#include "camera.h"

int initTexture(const char* path, unsigned int *texture);

void destroyTexture(unsigned int *texture);

SDL_FRect generateSprite(int ID, SDL_Point sheet, SDL_Point sprite);

#endif