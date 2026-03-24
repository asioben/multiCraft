#include "../include/graphic.h"

void vbo_init(unsigned int *VBO, void *data, size_t size){
    //Generate buffer ID
    glGenBuffers(1, VBO);
    //Bind 
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    //Copy the data into the buffer
    glBufferData(GL_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
}

void ebo_init(unsigned int *EBO, void *data, size_t size){
    //Generate buffer ID
    glGenBuffers(1, EBO);
    //Bind 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    //Copy the data into the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
}

void vertex_init(void){
    //vertex attribut
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //tex coord attribut
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //glBindVertexArray(0);
}

void instance_init(unsigned int VAO, unsigned int *VBO, void *data, size_t size){
    glBindVertexArray(VAO);
    //Generate buffer ID
    glGenBuffers(1, VBO);
    //Bind 
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    //Copy the data into the buffer
    glBufferData(GL_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
    //matrix attribut
    for(int i = 0; i < 4; i++){
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i,4,GL_FLOAT,GL_FALSE,sizeof(mat4),(void*)(i * sizeof(vec4)));
        glVertexAttribDivisor(2 + i,1);
    }
    glBindVertexArray(0);
}

void vbo_ebo_destroy(unsigned int *VBO, unsigned int *EBO){
    //Destroy the bufffer
    if(VBO != NULL)glDeleteBuffers(1,VBO);
    if(EBO != NULL)glDeleteBuffers(1,EBO);
}

void vao_init(unsigned int *VAO){
    //Generate 
    glGenVertexArrays(1,VAO);
    //Bind
    glBindVertexArray(*VAO);
}

void vao_destroy(unsigned int *VAO){
    //destroy
    if(VAO != NULL) glDeleteVertexArrays(1,VAO);
}

static int compileShader(const char *source, GLenum shaderType){
    unsigned int shader = 0;
    char *content = NULL;
    int value = 0;
    shader = glCreateShader(shaderType);
    //get the content of the src file
    value = readFile(source,&content);
    if(value == 1){
        printf("Erreur Compilation de Shader ;");
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

void render(Mesh *meshes, unsigned int program, unsigned int texture){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    glUniform1i(glGetUniformLocation(program,"ourTexture"),0);
    for(int i = 0; i < 3; i++){
        glBindVertexArray(meshes[i].VAO);
        //printf("%i\n",meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0,100);
    }
    glBindVertexArray(0);
}