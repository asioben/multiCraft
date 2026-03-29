#include "../include/chunk.h"

void generateChunk(Chunk *chunk){
    int counter = 0;
    chunk->meshesSize = 1;
    chunk->meshSize = malloc(chunk->meshesSize * sizeof(GLuint));
    chunk->types = malloc(chunk->meshesSize * sizeof(BlockID));
    for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_DEPTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                glm_mat4_identity(chunk->blocks[counter].model);
                vec3 position = {k*2,i*2,j*2};
                glm_translate(chunk->blocks[counter].model,position);
                chunk->blocks[counter].type = DIRT;
                counter += 1;
            }
        }
    }
    chunk->meshSize[0] = 8;
    chunk->types[0] = DIRT;
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