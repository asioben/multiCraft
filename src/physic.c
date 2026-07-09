#include "../include/physic.h"

bool raytrace(vec3 look, vec3 position, vec3 cube){
    vec3 toIntersect = {0.0f,0.0f,0.0f};
    glm_vec3_sub(cube,position,toIntersect);

    float length = glm_vec3_norm(toIntersect);
    printf("length: %f, cube: (%f,%f,%f)\n",length,cube[0],cube[1],cube[2]);
    //if(length >= 12) return false;

    vec3 ray = {0.0f,0.0f,0.0f};
    glm_vec3_sub(look,position,ray);
    float ray_length = glm_vec3_norm(ray);
    printf("ray length: %f\n",ray_length);
    float scale_factor = (length / ray_length) - 0.5f;
    printf("scale factor: %f\n",scale_factor);
    glm_vec3_scale(ray,scale_factor,ray);

    vec3 intersect = {0.0f,0.0f,0.0f};
    glm_vec3_add(position,ray,intersect);

    bool condition = false;

    if((intersect[0] >= cube[0] && intersect[0] <= cube[0] + 1.0f)
       && (intersect[1] >= cube[1] && intersect[1] <= cube[1] + 1.0f)
       && (intersect[2] >= cube[2] && intersect[2] <= cube[2] + 1.0f)){
        condition = true;
        printf("condition: %d\n",condition);
       }

    printf("look: %f,%f,%f\n",look[0],look[1],look[2]);
    printf("position: %f,%f,%f\n",position[0],position[1],position[2]);
    printf("ray: %f,%f,%f\n",ray[0],ray[1],ray[2]);
    printf("intersect: %f,%f,%f\n",intersect[0],intersect[1],intersect[2]);
    printf("/////////////////////\n");

    return condition;
}
