#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "graphic.h"
#include "perlin.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16

typedef struct{
    BlockID type;
    mat4 model;
}Block;

typedef struct{
    //blocks info
    Block *blocks;
    GLuint size;
    //chunk manager functions
    bool update;
    bool current;
    //mesh
    BlockID *types;
    GLuint *meshSize;
    GLuint meshesSize;
}Chunk;

void generateChunk(Chunk *chunk);

void generateMeshes(Chunk *chunk, Mesh **meshes, unsigned short *indices);

void destroyChunks(Chunk *chunks);

void destroyMeshes(Mesh **meshes, int size);

#endif