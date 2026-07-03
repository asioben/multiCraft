#include "../include/chunk.h"

int initBIDS(BIDS **types){
    if(*types != NULL){
         destroyBIDS(types); 
         (*types) = NULL;
    }
    if(*types == NULL){
        (*types) = malloc(1 * sizeof(BIDS));
        if(*types == NULL) return safe_return("BIDS is not allocated\n");
        (*types)->capacity = 3;
        (*types)->type = malloc((*types)->capacity * sizeof(BlockID));
        (*types)->type[0] = AIR;
        (*types)->type[1] = AIR;
        (*types)->type[2] = AIR;
        if((*types)->type == NULL) return safe_return("BIDS type ais not allocated\n");
        (*types)->sizes = calloc((*types)->capacity,sizeof(int));
        if((*types)->sizes == NULL) return safe_return("BIDS sizes is not allocated\n");
        (*types)->counter = -1;
    }
}

int insideBIDS(BIDS *types, BlockID type, int *element){
    for(int i = 0; i <= types->counter; i++){
        *element = i;
        if(types->type[i] == type) return 1;
    }
    return 0;
}

int updateBIDS(BIDS *types, BlockID *types_, int size, int *meshSize){
     for(int f = 0; f < size; f++){
        int element = 0;
        int inside = 0;
        BlockID type = AIR;
        int meshSize_ = 0;
        if(size > 1){
            type = *types_;
            meshSize_ = *meshSize;
            inside = insideBIDS(types,*types_,&element);
        }
        else{
            type = types_[f];
            meshSize_ = meshSize[f];
            inside = insideBIDS(types,types_[f],&element);
        }
        
        if(types->counter < 0 || inside != 1){
            types->counter += 1;
            if(types->counter >= types->capacity){
                types->capacity *= 2;
                BlockID *ptr1 = realloc(types->type,types->capacity * sizeof(BlockID));
                int *ptr2 = realloc(types->sizes,types->capacity * sizeof(int));
                if(ptr1 == NULL) return safe_return("Realloc types->types failed\n");
                else{types->type = ptr1;}
                if(ptr2 == NULL) return safe_return("Realloc types->sizes failes");
                else{
                    types->sizes = ptr2;
                    for(int i = types->counter; i < types->capacity; i++){
                        types->sizes[i] = 0;
                    }
                }
            }
            types->type[types->counter] = type;
            types->sizes[types->counter] += meshSize_;
            
        }else if(types->counter >= 0 && inside == 1){
            types->sizes[element] += meshSize_;
        }
    }
    return 1;
}

int generateChunk(Chunk *chunk, int seed){
    //if(chunk != NULL) return NULL;
    //printf("test\n");
    int counter = 0;
    int fullSize = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;
    chunk->meshesSize = 3;
    chunk->meshSize = calloc(chunk->meshesSize,sizeof(GLuint));
    if(chunk->meshSize == NULL) return safe_return("MeshSize is not allocated\n");
    chunk->types = malloc(chunk->meshesSize * sizeof(BlockID));
    if(chunk->types == NULL) return safe_return("types is not allocated\n");
    chunk->types[0] = GRASS;
    chunk->types[1] = DIRT;
    chunk->types[2] = STONE;
    chunk->blocks = malloc(fullSize * sizeof(Block));
    if(chunk->blocks == NULL) return safe_return("blocks is not allocated\n");
    chunk->models = NULL;
    chunk->minHeight = 0;

    for(int i = 0; i < CHUNK_WIDTH; i++){
        for(int j = 0; j < CHUNK_DEPTH; j++){
            for(int k = 0; k < CHUNK_HEIGHT; k++){
                vec3 position = {i,k,j};
                vec3 start = {chunk->start.x,0,chunk->start.z};
                glm_vec3_add(position,start,position);
                float noise = fractalPerlin2D(position[0],j,0.01f,10,0.5f,seed);
                int height = (int)((noise + 1.0f) * 15);
                if(k == height) {
                    chunk->blocks[counter].type = GRASS;
                    chunk->meshSize[0] += 1;
                }
                else if(k > height) chunk->blocks[counter].type = AIR;
                else if(k < height && k > (int)(height / 2)){
                    chunk->blocks[counter].type = DIRT; 
                    chunk->meshSize[1] += 1;
                }
                else if(k <= (int)(height / 2)){
                    chunk->blocks[counter].type = STONE; 
                    chunk->meshSize[2] += 1;
                }
                if(chunk->minHeight == 0 || height < chunk->minHeight) chunk->minHeight = height;
                chunk->blocks[counter].height = k;
                glm_mat4_identity(chunk->blocks[counter].model);
                glm_translate(chunk->blocks[counter].model,position);
                counter += 1;
            }
        }
    }

    //if(updateBIDS((*types),chunk->types,chunk->meshesSize,chunk->meshSize) == 0) return safe_return("BIDS update failed");
    
    chunk->size = counter;
    chunk->current = true;
    chunk->update = true;

    return 1;
}

int *generateVisibleBlocks(Chunk *chunk, int *blocks_size, BIDS *types){
    int size = CHUNK_WIDTH * CHUNK_DEPTH ;
    int *blocks =  malloc(size * sizeof(int));
    if(blocks == NULL) return NULL;
    int block_counter = -1;
    int meshSize = 1;
    //printf("héhé,%i\n",chunk->size);
    for(int i = 0; i < chunk->size; i++){
        //printf("0\n");
        if((chunk->blocks[i].height >= chunk->minHeight) && (chunk->blocks[i].type != AIR)){
            block_counter += 1;
            //printf("1\n");
            if(block_counter >= size){
               size *= 2;
               int *ptr = realloc(blocks,size * sizeof(int));
               //printf("2\n");
               if(ptr == NULL) return NULL;
               else{
                blocks = ptr;
            }
           }
           blocks[block_counter] = i;
           updateBIDS(types,&chunk->blocks[i].type,1,&meshSize);
        }
    }

    *blocks_size = block_counter;

    return blocks;
}

int generateMeshes(Chunk *chunk, BIDS *types){
    int blocks_size = -1;
    int before_counter = types->counter;
    if(before_counter < 0) before_counter = 0;
    int before_sizes[types->counter + 1];
    for(int v = 0; v <= types->counter; v++) before_sizes[v] = types->sizes[v];
    int *blocks = generateVisibleBlocks(chunk,&blocks_size,types);
    if(chunk->models != NULL) return 1;
    for(int w = 0; w < chunk->meshesSize; w++){
        chunk->meshSize[w] = 0;
        chunk->types[w] = AIR;
    }
    chunk->meshesSize = 0;
    if(before_counter < types->counter) chunk->meshesSize += (types->counter - before_counter);
    for(int x = 0; x <= types->counter; x++){
        if((types->sizes[x] - before_sizes[x]) > 0){
            chunk->meshesSize += 1;
            if(before_counter < x) chunk->meshSize[x] += types->sizes[x];
            else chunk->meshSize[x] += types->sizes[x] - before_sizes[x];
        }
    }
    if(chunk->models == NULL) chunk->models = malloc(chunk->meshesSize * sizeof(int*));
    if(chunk->models == NULL) return safe_return("Allocate models failed\n");
        
    
    //in case the chunk update we need to change that
    //PLEASE dont forget that 
    //pLEAAAse
    //my dumb aahh forgot...
    if((chunk->models) == NULL) return safe_return("Models failed\n");
    for(int i = 0; i < chunk->meshesSize; i++){
        chunk->models[i] = malloc(chunk->meshSize[i] * sizeof(int));
        chunk->types[i] = types->type[i];
        if(chunk->models[i] == NULL) return safe_return("Models failed AGAIN !\n");
            int counter = 0;
            for(int k = 0; k <= blocks_size; k++){
               if(chunk->blocks[blocks[k]].type == types->type[i]){
                 chunk->models[i][counter] = blocks[k];
                 counter += 1;
                }
             }
    }
    free(blocks);

    return 1;
}

int concatenateMeshes(Arena *arena, Chunk **chunk, Mesh **meshes, BIDS *types, int size, unsigned short *indices){
    if(*meshes != NULL){
       destroyMeshes(meshes,arena,types->counter + 1);
       printf("free\n");
       *meshes = NULL;
    }
    *meshes = (Mesh *)arena_alloc(arena,(types->counter + 1) * sizeof(Mesh));
    printf("alloc\n");
    if(*meshes == NULL) return safe_return("Allocation of meshes failed");

    for(int i = 0; i <= types->counter; i++){
        for (int j = 0; j < 36; j++) (*meshes)[i].indices[j] = indices[j];
        generateCube((*meshes)[i].vertices,types->type[i]);
        vao_init(&(*meshes)[i].VAO);
        vbo_init(&(*meshes)[i].VBO,(*meshes)[i].vertices,sizeof((*meshes)[i].vertices));
        ebo_init(&(*meshes)[i].EBO,(*meshes)[i].indices,sizeof((*meshes)[i].indices));
        vertex_init();
        int counter = 0;
        (*meshes)[i].size = types->sizes[i];
        (*meshes)[i].model = (mat4 *) arena_alloc(arena,types->sizes[i] * sizeof(mat4));
        if((*meshes)[i].model == NULL) return safe_return("Model failed");
        for(int j = 0; j < size; j++){
            for(int k = 0; k < chunk[j]->meshesSize; k++){
                //printf("here\n");
                if(chunk[j]->types[k] == types->type[i]){
                    //printf("0\n");
                    if(chunk[j]->meshSize[k] > 0) for(int m = 0; m < chunk[j]->meshSize[k]; m++){
                        //printf("%d\n",types->counter);
                        glm_mat4_copy(chunk[j]->blocks[chunk[j]->models[k][m]].model,(*meshes)[i].model[counter]);
                        //printf("2o\n");
                        counter += 1;
                    }
                }
            }
        }
        instance_init((*meshes)[i].VAO,&(*meshes)[i].instance,(*meshes)[i].model,sizeof(mat4) * types->sizes[i]);
    }

    return 1;
}

void destroyChunks(Chunk *chunks){
    //printf("%i\n",chunks->meshesSize);
        free(chunks->meshSize);
        free(chunks->types);
        free(chunks->blocks);
        for(int i = 0; i < chunks->meshesSize; i++){
            if(chunks->models != NULL) free(chunks->models[i]);
        }
        if(chunks->models != NULL)free(chunks->models);
}

void destroyMeshes(Mesh **meshes, Arena *arena, int size){
    for(int i = 0; i < size; i++){
        vbo_ebo_destroy(&(*meshes)[i].VBO,&(*meshes)[i].EBO);
        vbo_ebo_destroy(&(*meshes)[i].instance,NULL);
        vao_destroy(&(*meshes)[i].VAO);
        //free((*meshes)[i].model);
    }
    arena_reset(arena);
    //free((*meshes));
}

void destroyBIDS(BIDS **types){
    free((*types)->type);
    free((*types)->sizes);
    free(*types);
}