#include "../include/chunkManager.h"

int generateChunks(ChunkManager **chunk_, BIDS **types, int size){
    //int seed = random_(1,10);
    //printf("%d\n",seed);
    if((*chunk_) == NULL){
        (*chunk_) = malloc(1 * sizeof(ChunkManager));
        if(*chunk_ == NULL) return safe_return("Chunk manager allocation failed\n");
        (*chunk_)->chunks_size = size * size;
        (*chunk_)->chunks = malloc((*chunk_)->chunks_size * sizeof(Chunk));
        if((*chunk_)->chunks == NULL) return safe_return("Chunks allocation failed");
        (*chunk_)->update = true;
        (*chunk_)->load_size = 25;
        (*chunk_)->currentChunk = 0;
        (*chunk_)->loadChunks = malloc((*chunk_)->load_size * sizeof(Chunk));
        if((*chunk_)->loadChunks == NULL) return safe_return("'Load chunks' allocation failed");
    }
    for(int i = 0; i < (*chunk_)->chunks_size; i++){
        float x = (i%size)*CHUNK_WIDTH;
        float z = (floorf(i/size)*CHUNK_DEPTH);
        vec3s start = {x,0,z}; 
        (*chunk_)->chunks[i].start = start; 
        if(generateChunk(&(*chunk_)->chunks[i],(3000000)) == 0) return safe_return("Generation of the chunk failed\n");
    }
    vec3s position = { size * 4.0f,16.0f, size * 4.0f};
    getCurrentChunk(*chunk_,position);

    return 1;
}

int getCurrentChunk(ChunkManager *chunk_, vec3s position){
    float x = position.x;
    float z = position.z;

    chunk_->chunks[chunk_->currentChunk].current = false;

    int previous = chunk_->currentChunk;

    for(int i = 0; i < chunk_->chunks_size; i++){
        if(
          (x >= chunk_->chunks[i].start.x) &&
          (x < chunk_->chunks[i].start.x + CHUNK_WIDTH) &&
          (z >= chunk_->chunks[i].start.z)  &&
          (z < chunk_->chunks[i].start.z + CHUNK_DEPTH) 
        ){
            chunk_->chunks[i].current = true;
            chunk_->currentChunk = i;
            if(previous == chunk_->currentChunk) return 0;
            chunk_->update = true;
            //printf("%d\n",i);
            return 1;
        }
    }
    return 0;
}

int loadChunks(ChunkManager *chunk_, Arena *arena, BIDS **types, Mesh **meshes, unsigned short *indices){
    if(chunk_->update == true){
        chunk_->update = false;
        

        chunk_->load_size = 49;
   
        vec3s central_ = chunk_->chunks[chunk_->currentChunk].start;
        vec3 central = {central_.x,0,central_.z};
        vec3 square[chunk_->load_size];
        int size = (int)sqrt(chunk_->load_size);
        vec3 start = {central[0] - (floor(size/2)*CHUNK_WIDTH), 0, central[2] - (floor(size/2)*CHUNK_DEPTH)};


        for(int b = 0; b < chunk_->load_size; b++){
         float x = b % size;
         float z = floorf(b / size);
         square[b][0] = start[0] + (x * CHUNK_WIDTH);
         square[b][1] = 0.0f;
         square[b][2] = start[2] + (z * CHUNK_DEPTH);
        }

        int load_elements[chunk_->load_size];
        int counter = 0;
        for(int m = 0; m < chunk_->chunks_size; m++){
        
         for(int n = 0; n < chunk_->load_size; n++){
             vec3 check;
            
             glm_vec3_copy(chunk_->chunks[m].start.raw,check);
             if(glm_vec3_eqv(check,square[n])){
                 load_elements[counter] = m;
                 
                 counter ++;
                 break;
             }
         }
        }

        chunk_->load_size = counter;
          

        for(int d = 0; d < chunk_->load_size; d++){
         chunk_->loadChunks[d] = &chunk_->chunks[load_elements[d]];
        }
        //printf("bids\n");
        initBIDS(types);
        
        for(int y = 0; y < chunk_->load_size; y++){
          if(generateMeshes(chunk_->loadChunks[y],*types) == 0)safe_return("Meshes failed\n");
        
        }
        
        if(concatenateMeshes(arena,chunk_->loadChunks,meshes,*types,chunk_->load_size,indices) == 0) safe_return("Concatenation of meshes failed\n");

    }

    

    return 1;
}

int removeBlock(ChunkManager *chunk_, Camera *camera){
    for(int g = 0; g < chunk_->load_size; g++){
        for(int h = 0; h < chunk_->loadChunks[g]->meshesSize; h++){
            for(int d = 0; d < chunk_->loadChunks[g]->meshSize[h]; d++){
                vec3 cube_pos = {0.0f,0.0f,0.0f};
                glm_vec3_copy(chunk_->loadChunks[g]->blocks[chunk_->loadChunks[g]->models[h][d]].model[3],cube_pos);
                if(raytrace(camera->look.raw,camera->position.raw,cube_pos) == true){
                    printf("here: %f, %f, %f\n",cube_pos[0],cube_pos[1],cube_pos[2]);
                    return 1;
                }
            }
        }
    }
    return 0;
}

void destroyChunkManager(ChunkManager **chunk_){
    for(int i = 0; i < (*chunk_)->chunks_size; i++)destroyChunks(&(*chunk_)->chunks[i]);
    if((*chunk_)->loadChunks != NULL) {
        free((*chunk_)->loadChunks);
    }
    free(*chunk_);
}

/***
 * IMPLEMENTATION
 * I'll implement a new (not so)complex 
 * generation reallocation system
 * that would seamlessly reallocate
 * and generate new chunks
 * up to 225 chunks
 *  ***/