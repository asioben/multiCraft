#include "../include/physic.h"

static void getMouseRayWorld(mat4 invProjection, mat4 invView, vec4 rayClip, float *rayWorld){
    vec4 rayEye;
    glm_mat4_mulv(invProjection,rayClip,rayEye);
    vec4 tmp = {rayEye[0],rayEye[1],-1.0f,0.0f};
    glm_mat4_mulv(invView,tmp,rayWorld);
}

void screenToWorld(vec2 mouse, vec2 screen, mat4 View, mat4 Projection, float *ray){
    float x = (2.0f * mouse[0] / screen[0]) - 1.0f;
    float y = 1.0f - (2.0f * mouse[1] / screen[1]);

    vec4 rayClipStart = {x,y,-1.0f,1.0f};
    vec4 rayClipEnd = {x,y,1.0f,1.0f};

    mat4 VP;
    mat4 invVP;
    glm_mat4_mul(Projection,View,VP);
    glm_mat4_inv(VP,invVP);

    vec4 rayStart;
    glm_mat4_mulv(invVP,rayClipStart,rayStart);
    vec4 rayEnd;
    glm_mat4_mulv(invVP,rayClipEnd,rayEnd);

    vec3 rayWorldStart;
    glm_vec3_copy(rayStart,rayWorldStart);
    glm_vec3_scale(rayWorldStart,1.0f / rayStart[3],rayWorldStart);
    vec3 rayWorldEnd;
    glm_vec3_copy(rayEnd,rayWorldEnd);
    glm_vec3_scale(rayWorldEnd,1.0f / rayEnd[3],rayWorldEnd);

    glm_vec3_sub(rayWorldEnd,rayWorldStart,ray);
    glm_vec3_normalize(ray);

    //printf("ray: %f,%f,%f\n",ray[0],ray[1],ray[2]);
}

bool raytrace(vec3 ray, vec3 position, vec3 cube){

    vec3 min = {
        cube[0] ,
        cube[1] ,
        cube[2] 
    };

    vec3 max = {
        cube[0] + 1.0f,
        cube[1] + 1.0f,
        cube[2] + 1.0f
    };

    float tmin = -FLT_MAX;
    float tmax = FLT_MAX;

    for(int i = 0; i < 3; i++){
        float invDir = 1.0f / ray[i];

        float t1 = (min[i] - position[i]) * invDir;
        float t2 = (max[i] - position[i]) * invDir;

        if(t1 > t2){
            float tmp = t1;
            t1 = t2;
            t2 = tmp;
        }

        tmin = fmaxf(tmin,t1);
        tmax = fminf(tmax,t2);

        if(tmin > tmax) return false;
    }

    //if(tmax > 6.0f) return false;

    return tmax >= 0.0f;
}

