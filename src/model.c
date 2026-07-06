#include "../include/model.h"

float chunk_width = 16.0f;
float chunk_depth = 16.0f;

void generateTree(vec3s *positions, BlockID *blocks){
    //TRUNK PART
    int counter = 0;
    for(int i = 0; i < 7; i++){
        positions[counter + i].x = 0.0f;
        positions[counter + i].y = i;
        positions[counter + i].z = 0.0f;
        blocks[counter + i] = OAK;
    }
    counter = 7;

    //LEAVE PART

    //BASE
    for(int j = 0; j < 15; j++){
        int element = j + counter;
        int t = j;
        //if(j >= 12) t += 1;
        positions[element].x = t % 4;
        positions[element].y = 4;
        positions[element].z = floorf(t / 4);
        blocks[element] = LEAVES;

    }
    counter = 22;

    for(int j = 0; j < 8; j++){
        int element = j + counter;
        int t = j;
        //if(j >= 4) j += 1;
        positions[element].x = t % 3;
        positions[element].y = 5;
        positions[element].z = floorf(t / 3);
        blocks[element] = LEAVES;
    }
    counter = 30;

    for(int j = 0; j < 5; j++){
        
        int element = j + counter;
        positions[element].y = 6;
        if(j == 0){
            positions[element].x = 0;
            positions[element].z = -1;
            blocks[element] = LEAVES;
        }else if(j > 0 && j > 4){
            positions[element].x = (j - 1) - 1;
            positions[element].z = 0;
            blocks[element] = LEAVES;
        }else{
            positions[element].x = 0;
            positions[element].z = 1;
            blocks[element] = LEAVES;
        }
        
    }
}

void checkTreeValidPosition(vec3s *position, vec3s start){
    vec3s frontier[2] = {
        {start.x,start.y,start.z},
        {start.x + chunk_width,start.y,start.z + chunk_depth}
    };

    vec3s new_position[2] = {
        {position->x - 3.0f, position->y, position->z - 3.0f},
        {position->x + 3.0f, position->y, position->z + 3.0f}
    };

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            if(new_position[i].x >= frontier[j].x ){
                position->x -= 1.0f;
            }
            if(new_position[i].x < frontier[j].x ){
                position->x += 1.0f;
            }
            if(new_position[i].z >= frontier[j].z){
                position->z -= 1.0f;
            }
             if(new_position[i].z < frontier[j].z){
                position->z += 1.0f;
            }
        }
    }
}