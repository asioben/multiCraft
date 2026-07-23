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

    /*mat4 invView;
    mat4 invProjection;
    glm_mat4_inv(View,invView);
    glm_mat4_inv(Projection,invProjection);

    vec4 rayWordStart;
    getMouseRayWorld(invProjection,invView,rayClipStart,rayWordStart);

    vec4 rayWordEnd;
    getMouseRayWorld(invProjection,invView,rayClipEnd,rayWordEnd);

    glm_vec3_sub(rayWordEnd,rayWordStart,world);
    printf("start: %f,%f,%f\n",rayWordStart[0],rayWordStart[1],rayWordStart[2]);
    printf("end: %f,%f,%f\n",rayWordEnd[0],rayWordEnd[1],rayWordEnd[2]);*/
    //glm_normalize(world);
    //glm_mat4_mulv3(VP,ray,1.0f,world);

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

    /*vec4 world_ = {x,y,0.0f,1.0f};
    glm_mat4_mulv(invVP,world_,world_);
    glm_vec3_scale(world_,1/world_[3],world);

    printf("world: %f,%f,%f\n",world[0],world[1],world[2]);*/
    printf("ray: %f,%f,%f\n",ray[0],ray[1],ray[2]);
}

bool raytrace(vec3 ray, vec3 position, vec3 cube){
    /*vec3 toIntersect = {0.0f,0.0f,0.0f};
    glm_vec3_sub(cube,position,toIntersect);

    float length = glm_vec3_norm(toIntersect);
    printf("length: %f, cube: (%f,%f,%f)\n",length,cube[0],cube[1],cube[2]);
    if(length >= 12) return false;

    float ray_length = glm_vec3_norm(ray);
    printf("ray length: %f, ray: (%f,%f,%f)\n",ray_length,ray[0],ray[1],ray[2]);
    //float scale_factor = (length / ray_length);
    //printf("scale factor: %f\n",scale_factor);
    vec3 ray_;
    glm_vec3_scale(ray,length*0.8f,ray_);
   

    vec3 intersect = {0.0f,0.0f,0.0f};
    glm_vec3_add(ray_,mouse,intersect);

    bool condition = false;

    if((intersect[0] >= cube[0] - 0.5f && intersect[0] <= cube[0] + 0.5f)
       && (intersect[1] >= cube[1] - 0.5f && intersect[1] <= cube[1] + 0.5f)
       && (intersect[2] >= cube[2] - 0.5f && intersect[2] <= cube[2] + 0.5f)){
        condition = true;
        printf("condition: %d\n",condition);
    }

    printf("mouse: %f,%f,%f\n",mouse[0],mouse[1],mouse[2]);
    printf("position: %f,%f,%f\n",position[0],position[1],position[2]);
    printf("ray: %f,%f,%f\n",ray_[0],ray_[1],ray_[2]);
    printf("intersect: %f,%f,%f\n",intersect[0],intersect[1],intersect[2]);
    printf("/////////////////////\n");*/

    vec3 min = {
        cube[0] - 1.0f,
        cube[1] - 1.0f,
        cube[2] - 1.0f
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

    return tmax >= 0.0f;
}
