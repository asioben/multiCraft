#include "../include/camera.h"

mat4s getCamera(vec3s position, vec3s look){
    vec3s up = {0.0f,1.0f,0.0f};
    mat4s View = glms_lookat(position,look,up);

    return View;
}

mat4s worldMatrix(mat4s View){
    //create the model matrix, an identity matrix
    mat4s Model = glms_mat4_identity();
    //create the projection matrix
    mat4s Projection = glms_perspective(glm_rad(45.0f),(float)WIDTH/(float)HEIGHT,0.1f,100.0f);
    //create the world matrix
    mat4s VP = glms_mat4_mul(Projection,View);
    mat4s World =  glms_mat4_mul(VP,Model);

    return World;
}

void matrix_init(mat4 World, unsigned int program, unsigned int *matrix, int *counter){
    if(*counter == 0) {
        *matrix = glGetUniformLocation(program,"MVP");
        *counter = 1;
    }
    glUniformMatrix4fv(*matrix,1,GL_FALSE,&World[0][0]);
}