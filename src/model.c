#include "../include/model.h"

float chunk_width = 16.0f;
float chunk_depth = 16.0f;

void generateTree(vec3s *positions, BlockID *blocks){
    //TRUNK PART
    int counter = 0;
    for(int i = 0; i < 7; i++){
        int element = counter + i;
        positions[element].x = 0.0f;
        positions[element].y = i;
        positions[element].z = 0.0f;
        blocks[element] = OAK;
        //printf("(%f,%f,%f)\n",positions[element].x,positions[element].y,positions[element].z);
    }
    counter = 7;

    //LEAVE PART

    //BASE
    for(int j = 0; j < 24; j++){
        int element = j + counter;
        int t = j;
        if(j >= 12) t += 1;
        positions[element].x = (t % 5) - 2.0f;
        positions[element].y = 4.0f;
        positions[element].z = floorf(t / 5) - 2.0f;
        blocks[element] = LEAVES;

    }
    counter = 31;

    for(int j = 0; j < 8; j++){
        int element = j + counter;
        int t = j;
        if(j >= 4) t += 1;
        positions[element].x = (t % 3) - 1.0f;
        positions[element].y = 5.0f;
        positions[element].z = floorf(t / 3) - 1.0f;
        blocks[element] = LEAVES;
    }
    counter = 39;

    for(int j = 0; j < 5; j++){
        
        int element = j + counter;
        positions[element].y = 6.0f;
        if(j == 0){
            positions[element].x = 0.0f;
            positions[element].z = -1.0f;
            blocks[element] = LEAVES;
        }else if(j > 0 && j < 4){
            positions[element].x = (j - 1.0f) - 1.0f;
            positions[element].z = 0.0f;
            blocks[element] = LEAVES;
        }else{
            positions[element].x = 0.0f;
            positions[element].z = 1.0f;
            blocks[element] = LEAVES;
        }
        
    }

    counter = 44;

    positions[counter].x = 0.0f;
    positions[counter].y = 7.0f;
    positions[counter].z = 0.0f;
    blocks[counter] = LEAVES;
}

void checkTreeValidPosition(vec3s position, vec3s start, vec3 output){
    vec3s frontier[2] = {
        {start.x,start.y,start.z},
        {start.x + chunk_width,start.y,start.z + chunk_depth}
    };

    vec3s new_position[2] = {
        {position.x - 3.0f, position.y, position.z - 3.0f},
        {position.x + 3.0f, position.y, position.z + 3.0f}
    };

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            if(new_position[i].x >= frontier[j].x && j == 1){
                output[0] -= 2.0f;
            }
            if(new_position[i].x < frontier[j].x && j == 0){
                output[0] += 2.0f;
            }
            if(new_position[i].z >= frontier[j].z && j == 1){
                output[2] -= 2.0f;
            }
             if(new_position[i].z < frontier[j].z && j == 0){
                output[2] += 2.0f;
            }
        }
    }
}