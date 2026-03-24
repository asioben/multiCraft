#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <cglm/struct.h>

#include "utility.h"

typedef struct mesh_{
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    GLuint instance;
    float vertices[120];
    mat4 model[100];
    unsigned short indices[36];
    size_t indices_;
    size_t vertices_;
} Mesh;

//VBO
void vbo_init(unsigned int *VBO, void *data, size_t size);

void ebo_init(unsigned int *EBO, void *data, size_t size);

void instance_init(unsigned int VAO, unsigned int *VBO, void *data, size_t size);

void vertex_init(void);

void vbo_ebo_destroy(unsigned int *VBO, unsigned int *EBO);

//VAO
void vao_init(unsigned int *VAO);

void vao_destroy(unsigned int *VAO);

//SHADER
//unsigned int compileShader(const char *source, GLenum shaderType);

int shaders_init(const char *vSrc,const char *fSrc, int *handles);

void shaders_destroy(unsigned int vShader, unsigned int fShader, unsigned int program);

//RENDERING
void render(Mesh *meshes, unsigned int program, unsigned int texture);

#endif