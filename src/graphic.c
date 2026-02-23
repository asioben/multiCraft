#include "../include/graphic.h"

void vbo_init(unsigned int *VBO, float *data, size_t size){
    //Generate buffer ID
    glGenBuffers(1, VBO);
    //Bind 
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    //Copy the data into the buffer
    glBufferData(GL_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
}

void ebo_init(unsigned int *EBO, unsigned short *data, size_t size){
    //Generate buffer ID
    glGenBuffers(1, EBO);
    //Bind 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    //Copy the data into the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
    //vertex attribut
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
}

void vbo_ebo_destroy(unsigned int *VBO, unsigned int *EBO){
    //Destroy the bufffer
    glDeleteBuffers(1,VBO);
    glDeleteBuffers(1,EBO);
}

void vao_init(unsigned int *VAO){
    //Generate 
    glGenVertexArrays(1,VAO);
    //Bind
    glBindVertexArray(*VAO);
}

void vao_destroy(unsigned int *VAO){
    //destroy
    glDeleteVertexArrays(1,VAO);
}

static int compileShader(const char *source, GLenum shaderType){
    unsigned int shader = 0;
    char *content = NULL;
    int value = 0;
    shader = glCreateShader(shaderType);
    //get the content of the src file
    value = readFile(source,&content);
    if(value == 1){
        printf("Erreur Compilation de Shader ;(");
        return -1;
    }
    glShaderSource(shader,1,&content,NULL);
    glCompileShader(shader);
    free(content);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0);
    glBindVertexArray(0);
}