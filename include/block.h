#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"

#define BLOCKS_LIMIT 3

typedef enum {
    AIR,
    GRASS,
    DIRT,
    STONE
}BlockID;

void generateCube(float *vertices, BlockID block);

#endif