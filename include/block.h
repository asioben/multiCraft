#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"

#define BLOCKS_LIMIT 5

typedef enum {
    AIR,
    GRASS,
    DIRT,
    STONE,
    OAK,
    LEAVES
}BlockID;

void generateCube(float *vertices, BlockID block);

#endif