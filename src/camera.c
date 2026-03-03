#include "../include/camera.h"

void initCamera(Camera *camera, vec3s position, vec3s look){
    //general vectors
    vec3s forward = {0.0f,0.0f,-1.0f};
    vec3s right = {1.0f,0.0f,0.0f};
    vec3s up = {0.0f,1.0f,0.0f};
    //init
    camera->forward = forward;
    camera->right = right;
    camera->up = up;
    //position
    camera->position = position;
    camera->look = look;
    //angle
    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    //matrix
    //camera->View = glms_lookat(camera->position,camera->look,camera->up);
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

void matrix_init(mat4s View, unsigned int program, unsigned int *matrix, int *counter){
    mat4s World_ = worldMatrix(View);
    mat4 World;
    memcpy(World,World_.raw,sizeof(mat4));
    if(*counter == 0) {
        *matrix = glGetUniformLocation(program,"MVP");
        *counter = 1;
    }
    glUniformMatrix4fv(*matrix,1,GL_FALSE,&World[0][0]);
}

void cameraMovement(const Uint8 *keys, Mouse mouse, Camera *camera, Uint64 deltaTime){
    float sensibility = 0.003f;
    float time = 1.0f;//(.1 + deltaTime) * 0.1f;
    //printf("%f\n",time);
    //mouse
    camera->yaw += mouse.motion.x * time;
    if(camera->pitch > -1.57f && camera->pitch < 1.57f) camera->pitch += mouse.motion.y * time;
    //forward vector
    camera->forward.x = cos(camera->yaw) * cos(camera->pitch);
    camera->forward.y = sin(camera->pitch);
    camera->forward.z = sin(camera->yaw) * cos(camera->pitch);
    //printf("%f,%f,%f",camera->forward.x,camera->forward.y,camera->forward.z);
    camera->forward = glms_normalize(camera->forward);
    //right vector
    camera->right = glms_normalize(glms_cross(camera->forward,camera->up));
    //keyboard
    if(keys[SDL_SCANCODE_UP]) camera->position = glms_vec3_add(camera->position,camera->forward);
    if(keys[SDL_SCANCODE_DOWN]) camera->position = glms_vec3_sub(camera->position,camera->forward);
    if(keys[SDL_SCANCODE_RIGHT]) camera->position = glms_vec3_add(camera->position,camera->right);
    if(keys[SDL_SCANCODE_LEFT]) camera->position = glms_vec3_sub(camera->position,camera->right);

    camera->look = glms_vec3_add(camera->position,camera->forward);
}