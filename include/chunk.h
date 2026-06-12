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
    int height;
}Block;

typedef struct{
    BlockID *type;
    int counter;
    int capacity;
    int *sizes;
}BIDS; //Block Id struct that mimic a vector

typedef struct{
    //blocks info
    Block *blocks;
    GLuint size;
    //chunk manager functions
    bool update;
    bool current;
    vec3s start;
    //mesh
    BlockID *types;
    GLuint *meshSize;
    GLuint meshesSize;
    int **models;
    int minHeight;
}Chunk;

int initBIDS(BIDS **types);

int insideBIDS(BIDS *types, BlockID type, int *element);

int updateBIDS(BIDS *types, BlockID *types_, int size, int *meshSize);

int generateChunk(Chunk *chunk, BIDS **types, int seed);

int *generateVisibleBlocks(Chunk *chunk, int *blocks_size, BIDS *types);

int generateMeshes(Chunk *chunk, BIDS *types);

int concatenateMeshes(Chunk **chunk, Mesh **meshes, BIDS *types, int size, unsigned short *indices);

void destroyChunks(Chunk *chunks);

void destroyMeshes(Mesh **meshes, int size);

void destroyBIDS(BIDS **types);

#endif