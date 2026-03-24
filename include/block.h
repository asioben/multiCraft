#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"

typedef enum {
    GRASS,
    DIRT,
    STONE
}BlockID;

void generateCube(float *vertices, BlockID block);

#endif