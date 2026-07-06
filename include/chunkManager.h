#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "chunk.h"

#include "../extra_utilities/arena.h"

#define CHUNKS_LIMIT 64

typedef struct{
    //different kind of chunks
    Chunk **loadChunks;
    Chunk *chunks;

    // info such as sizes, etc...
    int chunks_size;
    int load_size;
    bool update;
    int currentChunk;
}ChunkManager;

int generateChunks(ChunkManager **chunk_, BIDS **types, int size);

int getCurrentChunk(ChunkManager *chunk_, vec3s position);

int loadChunks(ChunkManager *chunk_, Arena *arena, BIDS **types, Mesh **meshes, unsigned short *indices);

void destroyChunkManager(ChunkManager **chunk_);

#endif