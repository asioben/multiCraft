#include "../include/scene.h"

//function prototype (static one)
static void renderTitle(unsigned int VAO, unsigned int program, unsigned int texture);

void sceneManagement(Scene *scene, Mesh *meshes, unsigned int program, unsigned int texture, unsigned int VAO){
    switch(*scene){
        case TITLE:{
            scene = TITLE;
            renderTitle(VAO,program,texture);
        }break;
        case GAME: {
            glClearColor(0.529f,0.807f,0.922f,1.0f);
            *scene = GAME;
            //if(initTexture("/Users/user/Developer/multiCraft/sprites/block.png",&texture)==1) return safe_exit("Error Texture",window,context);
            render(meshes,BLOCKS_LIMIT,program,texture);
        }break;
    }
}

int initTitle(unsigned int *VAO, unsigned int *VBO, unsigned int *texture){
    float vertices[] = {
    -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // Top-Left
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom-Left
     1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Bottom-Right

    -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // Top-Left
     1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Bottom-Right
     1.0f,  1.0f, 0.0f, 1.0f, 0.0f  // Top-Right
    
    };

    //short indices[] = {0,1,2};

    if(initTexture("/Users/user/Developer/multiCraft/sprites/title.png",texture) == 1) return 0;

    vao_init(VAO);
    vbo_init(VBO,vertices,sizeof(vertices));
    //ebo_init()
    vertex_init();

    //printf("%d\n",*VAO);

    return 1;
}

static void renderTitle(unsigned int VAO, unsigned int program, unsigned int texture){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDepthMask(GL_FALSE);
    //glUseProgram(program);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D,texture);
    //glUniform1i(glGetUniformLocation(program,"ourTexture"),0);
    //printf("%d\n",VAO);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawElements(GL_)

}

void render(Mesh *meshes, int size, unsigned int program, unsigned int texture){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    glUniform1i(glGetUniformLocation(program,"ourTexture"),0);
    for(int i = 0; i < size; i++){
        glBindVertexArray(meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0,meshes[i].size);
    }
    glBindVertexArray(0);
}
