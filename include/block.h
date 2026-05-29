#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"

typedef enum {
    AIR,
    GRASS,
    DIRT,
    STONE
}BlockID;

void generateCube(float *vertices, BlockID block);

#endif