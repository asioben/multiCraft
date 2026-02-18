#include "../include/graphic.h"

void vbo_init(unsigned int *VBO, float *vertices, size_t size){
    //Generate buffer ID
    glGenBuffers(1, VBO);
    //Bind 
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    //Copy the data into the buffer
    glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
}

void vbo_destroy(unsigned int *VBO){
    //Destroy the bufffer
    glDeleteBuffers(1,VBO);
}

void vao_init(unsigned int *VAO){
    //Generate 
    glGenVertexArrays(1,VAO);
    //Bind
    glBindVertexArray(*VAO);
    //vertex attribut
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
}

void vao_destroy(unsigned int *VAO){
    //destroy
    glDeleteVertexArrays(1,VAO);
}

static int compileShader(const char *source, GLenum shaderType){
    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader,1,&source,NULL);
    glCompileShader(shader);
    //check for failure
    int failure;
    char info[512];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&failure);
    if(!failure){
        glGetShaderInfoLog(shader,512,NULL,info);
        printf("Erreur Compilation de Shader: %s\n",info);
        return -1;
    }
    
    return shader;
}

int shaders_init(const char *vSrc,const char *fSrc, int *handles){
    //compile the shaders
    int vShader,fShader;
    //check for each shader if the compilation failed
    vShader = compileShader(vSrc,GL_VERTEX_SHADER);
    if(vShader == -1) return 1;
    fShader = compileShader(fSrc,GL_FRAGMENT_SHADER);
    if(fShader == -1) return 1;
    //create the program with the compiled shaders
    unsigned int program;
    program = glCreateProgram();
    glAttachShader(program,vShader);
    glAttachShader(program,fShader);
    glLinkProgram(program);
    glUseProgram(program);
    //check for failure
    int failure;
    char info[512];
    glGetProgramiv(program,GL_LINK_STATUS,&failure);
    if(!failure){
        glGetProgramInfoLog(program,512,NULL,info);
        printf("Erreur de Shader: %s\n",info);
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return 1;
    }
    //get different handles
    handles[0] = program;
    handles[1] = vShader;
    handles[2] = fShader;

    return 0;
}

void shaders_destroy(unsigned int vShader, unsigned int fShader, unsigned int program){
    glDeleteProgram(program);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void render(unsigned int VAO, unsigned int program){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,3);
}