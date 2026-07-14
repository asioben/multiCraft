#include "../include/chunk.h"
#include "../include/model.h"

int fullSize = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;

int initBIDS(BIDS **types){
    if(*types != NULL){
         destroyBIDS(types); 
         (*types) = NULL;
    }
    if(*types == NULL){
        (*types) = malloc(1 * sizeof(BIDS));
        if(*types == NULL) return safe_return("BIDS is not allocated\n");
        (*types)->capacity = 5;
        (*types)->type = malloc((*types)->capacity * sizeof(BlockID));
        (*types)->type[0] = AIR;
        (*types)->type[1] = AIR;
        (*types)->type[2] = AIR;
        (*types)->type[3] = AIR;
        (*types)->type[4] = AIR;
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
    chunk->meshesSize = 5;
    chunk->meshSize = calloc(chunk->meshesSize,sizeof(GLuint));
    if(chunk->meshSize == NULL) return safe_return("MeshSize is not allocated\n");
    chunk->types = malloc(chunk->meshesSize * sizeof(BlockID));
    if(chunk->types == NULL) return safe_return("types is not allocated\n");
    chunk->types[0] = GRASS;
    chunk->types[1] = DIRT;
    chunk->types[2] = STONE;
    chunk->types[3] = OAK;
    chunk->types[4] = LEAVES;
    chunk->blocks = malloc(fullSize * sizeof(Block));
    if(chunk->blocks == NULL) return safe_return("blocks is not allocated\n");
    chunk->models = NULL;
    chunk->minHeight = 0;

    int number_of_tree = 1;
    int tree_created = 0;
    vec3s tree_positions[45];
    BlockID tree_blocks[45];
    int tree_counter = 0;

    //there is a reason behind this if statement
    if(tree_created < number_of_tree){
                    vec3 translation = {random_(0,10),random_(12,14),random_(0,10)};
                    tree_created += 1;
                    generateTree(tree_positions,tree_blocks);
                    checkTreeValidPosition(tree_positions[0],chunk->start,translation);
                    for(int s = 0; s < 45; s++){
                        vec3 tree_position_ = {tree_positions[s].x,tree_positions[s].y,tree_positions[s].z};
                       
                        glm_vec3_add(tree_position_,translation,tree_position_);
                        tree_positions[s].x = tree_position_[0];
                        tree_positions[s].y = tree_position_[1];
                        tree_positions[s].z = tree_position_[2];
                        
                    }
                }

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
                }for(int u = 0; u < 45; u++){
                    if( tree_positions[u].x == (float)i && tree_positions[u].z == (float)j && tree_positions[u].y == (float)k){
                       chunk->blocks[counter].type = tree_blocks[u];
                       if(tree_blocks[u] == OAK){
                         chunk->meshSize[3] += 1;
                        }
                        else if(tree_blocks[u] == LEAVES) {
                        chunk->meshSize[4] += 1;
                        }
                   }
                } 
                
                if(chunk->minHeight == 0 || height < chunk->minHeight) chunk->minHeight = height;
                chunk->blocks[counter].height = k;
                glm_mat4_identity(chunk->blocks[counter].model);
                glm_translate(chunk->blocks[counter].model,position);
                counter ++;
            }
        }
    }

    //if(updateBIDS((*types),chunk->types,chunk->meshesSize,chunk->meshSize) == 0) return safe_return("BIDS update failed");
    
    chunk->size = counter;
    chunk->current = false;
    chunk->update = false;

    return 1;
}

static bool checkOccludedBlock(Block *blocks, int element){
    int elements[6] = {
        element + 1,
        element - 1,
        element + CHUNK_HEIGHT,
        element - CHUNK_HEIGHT,
        element + (CHUNK_DEPTH * CHUNK_HEIGHT),
        element - (CHUNK_DEPTH * CHUNK_HEIGHT)
    };

    int occluder = 0;

    int y = element % CHUNK_HEIGHT;
    int z = (element / CHUNK_HEIGHT) % CHUNK_DEPTH;
    int x = element / (CHUNK_HEIGHT * CHUNK_DEPTH);

    for(int i = 0; i < 6; i++){
        if((elements[i] >= 0 && elements[i] < fullSize)){
            if(x == 0 || z == 0 || x == CHUNK_WIDTH - 1 || z == CHUNK_DEPTH - 1) return false;
            if(blocks[elements[i]].type == AIR){
                return false;
            }else{
               occluder ++;
            }
        }
    }

    
    if(occluder == 6) return true;
    return false;
}

int *generateVisibleBlocks(Chunk *chunk, int *blocks_size, BIDS *types){
    int size = CHUNK_WIDTH * CHUNK_DEPTH ;
    int *blocks =  malloc(size * sizeof(int));
    if(blocks == NULL) return NULL;
    int block_counter = -1;
    int meshSize = 1;
    //printf("ok: %d\n",chunk->blocks[3819].type);
    for(int i = 0; i < chunk->size; i++){
        if((chunk->blocks[i].height >= chunk->minHeight)
         && (chunk->blocks[i].type != AIR) 
         && (checkOccludedBlock(chunk->blocks,i) == false)){
            block_counter += 1;
            if(block_counter >= size){
               size *= 2;
               int *ptr = realloc(blocks,size * sizeof(int));
               if(ptr == NULL) return NULL;
               else{
                blocks = ptr;
            }
           }
           blocks[block_counter] = i;
           updateBIDS(types,&chunk->blocks[i].type,1,&meshSize);
           
        }
    }
    if(*blocks_size != NULL) *blocks_size = block_counter;

    return blocks;
}

int generateMeshes(Chunk *chunk, BIDS *types){
    if(chunk->models != NULL && chunk->update != true) return 1;
    int blocks_size = -1;
    int before_counter = types->counter;
    if(before_counter < 0) before_counter = 0;
    int before_sizes[BLOCKS_LIMIT];
    for(int v = 0; v < BLOCKS_LIMIT; v++){
         before_sizes[v] = types->sizes[v];
    }
    int *blocks = generateVisibleBlocks(chunk,&blocks_size,types);
    for(int w = 0; w < chunk->meshesSize; w++){
        chunk->meshSize[w] = 0;
        chunk->types[w] = AIR;
    }
    chunk->meshesSize = 0;
    //if(before_counter < types->counter) chunk->meshesSize += (types->counter - before_counter);
    for(int x = 0; x <= types->counter; x++){
       
        if((types->sizes[x] - before_sizes[x]) > 0){
            chunk->meshesSize += 1;
            //printf("we are %d\n",chunk->meshesSize);
            if(before_counter < x) chunk->meshSize[x] += types->sizes[x];
            else chunk->meshSize[x] += types->sizes[x] - before_sizes[x];
        }
    }
    
    if(chunk->models == NULL) chunk->models = malloc(chunk->meshesSize * sizeof(int*));
    //else return safe_return("Already allocated\n");
    //if(chunk->update == true) chunk->models = realloc(chunk->meshesSize * sizeof(int *));
    if(chunk->models == NULL) return safe_return("Allocate models failed\n");
        
   // printf("n: %d\n",chunk->meshesSize);
    //in case the chunk update we need to change that
    //PLEASE dont forget that 
    //pLEAAAse
    //my dumb aahh forgot...
    //UPDATE
    //I think I remember what I needed to change
    //it caused me a very BIG issue :( !!!!!
    for(int i = 0; i < chunk->meshesSize; i++){
        //printf("%d\n",types->type[i]);
        if(chunk->update != true){
            chunk->models[i] = malloc(chunk->meshSize[i] * sizeof(int));
            if(chunk->models[i] == NULL) return safe_return("Models failed AGAIN !\n");
            //printf("here at first %d\n",i);
        }else{
            mat4 *ptr = realloc(chunk->models[i],chunk->meshSize[i] * sizeof(int));
            if(ptr == NULL) return safe_return("Reallocation of models failed\n");
            else{
                chunk->models[i] = ptr;
                //printf("then here %d\n",i);
            }
        }
        chunk->types[i] = types->type[i];
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

int concatenateMeshes(Chunk **chunk, Mesh **meshes, BIDS *types, int size, unsigned short *indices){
    if(*meshes != NULL){
        //it depends on the number of meshes it had 
       destroyMeshes(meshes,types->counter + 1);
    }
    *meshes = malloc((types->counter + 1) * sizeof(Mesh));
    if(*meshes == NULL) return safe_return("Allocation of meshes failed");
    for(int i = 0; i <= types->counter; i++){
        if(types->type[i] == 1) printf("y:%d\n",types->sizes[i]);
        for (int j = 0; j < 36; j++) (*meshes)[i].indices[j] = indices[j];
        generateCube((*meshes)[i].vertices,types->type[i]);
        vao_init(&(*meshes)[i].VAO);
        vbo_init(&(*meshes)[i].VBO,(*meshes)[i].vertices,sizeof((*meshes)[i].vertices));
        ebo_init(&(*meshes)[i].EBO,(*meshes)[i].indices,sizeof((*meshes)[i].indices));
        vertex_init();
        int counter = 0;
        (*meshes)[i].size = types->sizes[i];
        (*meshes)[i].model = malloc(types->sizes[i] * sizeof(mat4));
        if((*meshes)[i].model == NULL) return safe_return("Model failed");
        for(int j = 0; j < size; j++){
            for(int k = 0; k < chunk[j]->meshesSize; k++){
                if(chunk[j]->types[k] == types->type[i]){
                    if(chunk[j]->meshSize[k] > 0) for(int m = 0; m < chunk[j]->meshSize[k]; m++){
                        glm_mat4_copy(chunk[j]->blocks[chunk[j]->models[k][m]].model,(*meshes)[i].model[counter]);
                        counter += 1;
                    }
                }
            }
        }
        
        instance_init((*meshes)[i].VAO,&(*meshes)[i].instance,(*meshes)[i].model,sizeof(mat4) * types->sizes[i]);
    }

    return 1;
}

static int meshesSmallRoutine(Mesh **meshes, int element){
    mat4 *ptr_ = realloc((*meshes)[element].model,(*meshes)[element].size * sizeof(mat4));
        if(ptr_ == NULL) return safe_return("Reallocation of meshes model failed\n");
        else{
            (*meshes)[element].model = ptr_;
            //printf("ici\n");
        }
        //if((*meshes)[element].model == NULL) return safe_return("Allocation for model in meshes struct\n");
        vao_init(&(*meshes)[element].VAO);
        vbo_init(&(*meshes)[element].VBO,(*meshes)[element].vertices,sizeof((*meshes)[element].vertices));
        ebo_init(&(*meshes)[element].EBO,(*meshes)[element].indices,sizeof((*meshes)[element].indices));
        vertex_init();

        return 1;
}

static void copyModelsRoutine(mat4 *models, Mesh **meshes, int shift, int element, int index){
    for(int j = 0; j < (*meshes)[element].size; j++){
        int index_ = j;
        int cond_count = 0;

        if(j >= index) index_ = j + shift;
        glm_mat4_copy((*meshes)[element].model[j],models[index_]);
    }

}

int updateMeshes(Chunk *chunk, Mesh **meshes, BIDS *types, int event, int element, int index){
    BlockID type_id = chunk->blocks[element].type;
    int id_type = 0;
    for(int i = 0; i <= types->counter; i++){
        if(types->type[i] == type_id){
            id_type = i; 
            break; 
        }
    }
    chunk->blocks[element].type = AIR;
    //printf("%d,%d\n",element,index);

    //i'll generalize later
    mat4 *models = malloc(((*meshes)[id_type].size + event) * sizeof(mat4));

    copyModelsRoutine(models,meshes,event,id_type,index);

    vbo_ebo_destroy(&(*meshes)[id_type].VBO,&(*meshes)[id_type].EBO);
    vbo_ebo_destroy(&(*meshes)[id_type].instance,NULL);
    vao_destroy(&(*meshes)[id_type].VAO);

    //
    //printf("0:%d\n",(*meshes)[id_type].size);

    int chunk_sizes_holder[chunk->meshesSize];

    // REMOVE 
    if(event == -1){
        (*meshes)[id_type].size -= 1;
        if(meshesSmallRoutine(meshes,id_type) == 0) return 0;
        //printf("1:%d\n",(*meshes)[id_type].size);
        for(int i = 0; i < chunk->meshesSize; i++){
            chunk_sizes_holder[i] = types->sizes[i];
            //printf("types: %d\n",types->sizes[i]);
            types->sizes[i] -= chunk->meshSize[i];
            
        }
        chunk->update = true;
        generateMeshes(chunk,types);
        for(int i = 0; i < chunk->meshesSize; i++){
            int diff = types->sizes[i] - chunk_sizes_holder[i];
            //printf("again %d\n",types->sizes[i]);
            if(diff == 1){
            int idx = 0;
               printf("touché, %d,%d,%d\n",diff,i,types->type[i]);
            
               for(int j = 0; j < chunk->meshSize[i]; j++){
                //printf("testons: %d\n",chunk->blocks[chunk->models[i][j]].type);
                if(glm_vec3_eqv(chunk->blocks[chunk->models[i][j]].model[3],(*meshes)[i].model[j][3]) == false){
                    printf("its true %d\n",j);
                    idx = chunk->models[i][j];
                    break;
                } 
               }
               vbo_ebo_destroy(&(*meshes)[i].VBO,&(*meshes)[i].EBO);
               vbo_ebo_destroy(&(*meshes)[i].instance,NULL);
               vao_destroy(&(*meshes)[i].VAO);
               (*meshes)[i].size += 1;
               if(meshesSmallRoutine(meshes,i) == 0) return 0;
               //printf("%d,%d,%d\n",(*meshes)[i].model[0][3][0],(*meshes)[i].model[0][3][1],(*meshes)[i].model[0][3][2]);

               glm_mat4_copy(chunk->blocks[idx].model,(*meshes)[i].model[(*meshes)[i].size - 1]);
               instance_init((*meshes)[i].VAO,&(*meshes)[i].instance,(*meshes)[i].model,sizeof(mat4) * (*meshes)[i].size);
            }
        }

    }

    for(int k = 0; k < (*meshes)[id_type].size; k++){
        //printf("%p\n",models);
        glm_mat4_copy(models[k],(*meshes)[id_type].model[k]);
    }

    instance_init((*meshes)[id_type].VAO,&(*meshes)[id_type].instance,(*meshes)[id_type].model,sizeof(mat4) * (*meshes)[id_type].size);

    free(models);

    //chunk->update = true;

    return 0;

}

void destroyChunks(Chunk *chunks){
        free(chunks->meshSize);
        free(chunks->types);
        free(chunks->blocks);
        for(int i = 0; i < chunks->meshesSize; i++){
            if(chunks->models != NULL) free(chunks->models[i]);
        }
        if(chunks->models != NULL)free(chunks->models);
}

void destroyMeshes(Mesh **meshes, int size){
    for(int i = 0; i < size; i++){
        vbo_ebo_destroy(&(*meshes)[i].VBO,&(*meshes)[i].EBO);
        vbo_ebo_destroy(&(*meshes)[i].instance,NULL);
        vao_destroy(&(*meshes)[i].VAO);
        free((*meshes)[i].model);
    }
    //arena_reset(arena);
    free((*meshes));
}

void destroyBIDS(BIDS **types){
    free((*types)->type);
    free((*types)->sizes);
    free(*types);
}

/**
 * For dynamic
 * I'd only update the updated chunk
 * some note:
 * 1: chunk->minHeight should be checked
 * I should lower it every time we lower the height 
 * that's been rendered
 */