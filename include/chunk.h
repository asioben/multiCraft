#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "graphic.h"

#define CHUNK_WIDTH 2
#define CHUNK_HEIGHT 2
#define CHUNK_DEPTH 2

typedef struct{
    BlockID type;
    mat4 model;
}Block;

typedef struct{
    //blocks info
    Block blocks[CHUNK_DEPTH*CHUNK_HEIGHT*CHUNK_WIDTH];
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