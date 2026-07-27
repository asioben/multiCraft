#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"

#define BLOCKS_LIMIT 6

typedef enum {
    AIR,
    GRASS,
    DIRT,
    STONE,
    OAK,
    LEAVES,
    BEDROCK
}BlockID;

void generateCube(float *vertices, BlockID block);

void pickBlock(const Uint8 *keys, BlockID *block);

#endif