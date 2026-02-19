#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "utility.h"

//VBO
void vbo_init(unsigned int *VBO, float *data, size_t size);

void ebo_init(unsigned int *EBO, unsigned short *data, size_t size);

void vbo_ebo_destroy(unsigned int *VBO, unsigned int *EBO);

//VAO
void vao_init(unsigned int *VAO);

void vao_destroy(unsigned int *VAO);

//SHADER
//unsigned int compileShader(const char *source, GLenum shaderType);

int shaders_init(const char *vSrc,const char *fSrc, int *handles);

void shaders_destroy(unsigned int vShader, unsigned int fShader, unsigned int program);

//RENDERING
void render(unsigned int VAO, unsigned int program);

#endif