#include "../include/chunk.h"

void generateChunk(Chunk *chunk){
    int counter = 0;
    chunk->meshesSize = 3;
    chunk->meshSize = malloc(chunk->meshesSize * sizeof(GLuint));
    chunk->types = malloc(chunk->meshesSize * sizeof(BlockID));
    chunk->types[0] = GRASS;
    chunk->types[1] = DIRT;
    chunk->types[2] = STONE;
    chunk->meshSize[0] = 0;
    chunk->meshSize[1] = 0;
    chunk->meshSize[2] = 0;
    chunk->blocks = malloc(CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * sizeof(Block));

    for(int i = 0; i < CHUNK_WIDTH; i++){
        for(int j = 0; j < CHUNK_DEPTH; j++){
            for(int k = 0; k < CHUNK_HEIGHT; k++){
                float noise = fractalPerlin2D(i,j,0.01f,5,0.5f,1337);
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
                glm_mat4_identity(chunk->blocks[counter].model);
                vec3 position = {i,k,j};
                glm_translate(chunk->blocks[counter].model,position);
                counter += 1;
            }
        }
    }
    
    chunk->size = counter;
    chunk->current = true;
    chunk->update = true;
}

void generateMeshes(Chunk *chunk, Mesh **meshes, unsigned short *indices){
    *meshes = malloc(chunk->meshesSize * sizeof(Mesh));
    for(int i = 0; i < chunk->meshesSize; i++){
        for (int j = 0; j < 36; j++) (*meshes)[i].indices[j] = indices[j];
        generateCube((*meshes)[i].vertices,chunk->types[i]);
        vao_init(&(*meshes)[i].VAO);
        vbo_init(&(*meshes)[i].VBO,(*meshes)[i].vertices,sizeof((*meshes)[i].vertices));
        ebo_init(&(*meshes)[i].EBO,(*meshes)[i].indices,sizeof((*meshes)[i].indices));
        vertex_init();
        (*meshes)[i].size = chunk->meshSize[i];
        (*meshes)[i].model = malloc(chunk->meshSize[i] * sizeof(mat4));
        int counter = 0;
        for(int k = 0; k < chunk->size; k++){
            if(chunk->blocks[k].type == chunk->types[i]){
                glm_mat4_copy(chunk->blocks[k].model,(*meshes)[i].model[counter]);
                counter += 1;
            }
        }
        instance_init((*meshes)[i].VAO,&(*meshes)[i].instance,(*meshes)[i].model,sizeof(mat4) * chunk->meshSize[i]);
    }
}

void destroyChunks(Chunk *chunks){
        free(chunks->meshSize);
        free(chunks->types);
        free(chunks->blocks);
}

void destroyMeshes(Mesh **meshes, int size){
    for(int i = 0; i < size; i++){
        vbo_ebo_destroy(&(*meshes)[i].VBO,&(*meshes)[i].EBO);
        vbo_ebo_destroy(&(*meshes)[i].instance,NULL);
        vao_destroy(&(*meshes)[i].VAO);
        free((*meshes)[i].model);
    }
    free((*meshes));
}