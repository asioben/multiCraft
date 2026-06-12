#include "../include/chunkManager.h"

int generateChunks(ChunkManager **chunk_, BIDS **types, int size){
    //int seed = random_(1,10);
    if((*chunk_) == NULL){
        (*chunk_) = malloc(1 * sizeof(ChunkManager));
        if(*chunk_ == NULL) return safe_return("Chunk manager allocation failed\n");
        (*chunk_)->chunks_size = size * size;
        (*chunk_)->chunks = malloc((*chunk_)->chunks_size * sizeof(Chunk));
        if((*chunk_)->chunks == NULL) return safe_return("Chunks allocation failed");
        (*chunk_)->loadChunks = NULL;
        (*chunk_)->update = true;
        (*chunk_)->loadChunks = NULL;
        (*chunk_)->load_size = 1;
        (*chunk_)->currentChunk = 4;
    }
    for(int i = 0; i < (*chunk_)->chunks_size; i++){
        float x = (i%size)*CHUNK_WIDTH;
        float z = (floor(i/size)*CHUNK_DEPTH);
        vec3s start = {x,0,z}; 
        (*chunk_)->chunks[i].start = start; 
        if(generateChunk(&(*chunk_)->chunks[i],types,(3000000)) == 0) return safe_return("Generation of the chunk failed\n");
    }
    /*vec3s position = {size * 8.0f,16.0f,size * 8.0f};
    getCurrentChunk(*chunk_,position);*/

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
            return 1;
        }
    }
    return 0;
}

int loadChunks(ChunkManager *chunk_, BIDS **types, Mesh **meshes, unsigned short *indices){
    if(chunk_->update == true){
        chunk_->update == false;
        if(chunk_->loadChunks == NULL) chunk_->loadChunks = malloc(chunk_->load_size * sizeof(Chunk *));
        if(chunk_->loadChunks == NULL) return safe_return("Allocation of loading chunks failed");
        
        //printf("I was here%p\n",(void*)(chunk_)->loadChunks);
        for(int j = 0; j < chunk_->load_size; j++){
            /*if(chunk_->currentChunk == 0){
                chunk_->loadChunks[j] = &chunk_->chunks[j];
                printf("a%i\n",j);
            }else if(chunk_->currentChunk < chunk_->chunks_size){
                int element = (chunk_->currentChunk+(j-1));
                chunk_->loadChunks[j] = &chunk_->chunks[element];
                printf("b%i\n",element);
            }else{
                int element = (chunk_->currentChunk-j);
                chunk_->loadChunks[j] = &chunk_->chunks[j];
                printf("c%i\n",element);
            }*/
           initBIDS(types);
           //printf("%i\n",chunk_->currentChunk);
           //DO NOT REMOVE printf YET
           //MY CODE ONLY WORKS WHEN I CALL IT
           printf("");
           chunk_->loadChunks[j] = &chunk_->chunks[chunk_->currentChunk];
           //printf("%i\n",chunk_->loadChunks[j]->meshesSize);
           
        }
        
        for(int y = 0; y < chunk_->load_size; y++){
           if(generateMeshes(chunk_->loadChunks[y],*types) == 0)safe_return("Meshes failed\n");  
        }
    
        if(concatenateMeshes(chunk_->loadChunks,meshes,*types,chunk_->load_size,indices) == 0) safe_return("Concatenation of meshes failed\n");
    

    }

    return 1;
}

void destroyChunkManager(ChunkManager **chunk_){
    for(int i = 0; i < (*chunk_)->chunks_size; i++)destroyChunks(&(*chunk_)->chunks[i]);
    if((*chunk_)->loadChunks != NULL) {
        free((*chunk_)->loadChunks);
    }
    free(*chunk_);
}