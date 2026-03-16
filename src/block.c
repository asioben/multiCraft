#include "../include/block.h"

void generateCube(float *vertices, BlockID block){
    SDL_Point sheet = {384,704};
    SDL_Point sprite = {16,16};
    SDL_FRect texCoords[6];
    int spritesID[6] = {0,0,0,0,0,0};

    switch(block){
        case GRASS:{
            spritesID[0] = 3;
            spritesID[1] = 3;
            spritesID[2] = 0;
            spritesID[3] = 2;
            spritesID[4] = 3;
            spritesID[5] = 3;
        }break;
        case DIRT:{
            spritesID[0] = 2;
            spritesID[1] = 2;
            spritesID[2] = 2;
            spritesID[3] = 2;
            spritesID[4] = 2;
            spritesID[5] = 2;
        }break;
        default:{
            printf("Default texture");
            spritesID[0] = 3;
            spritesID[1] = 3;
            spritesID[2] = 0;
            spritesID[3] = 2;
            spritesID[4] = 3;
            spritesID[5] = 3;
        }
    }

    for(int i = 0; i < 6; i++) texCoords[i] = generateSprite(spritesID[i],sheet,sprite); 
    
    float vertice_[] = {
        0.0f, 0.0f, 0.0f,texCoords[0].x, texCoords[0].y + texCoords[0].h,
        0.0f,  1.0f, 0.0f,texCoords[0].x, texCoords[0].y,
         1.0f,  1.0f, 0.0f,texCoords[0].x + texCoords[0].w, texCoords[0].y,
         1.0f, 0.0f, 0.0f,texCoords[0].x + texCoords[0].w, texCoords[0].y + texCoords[0].h,
   
        // Back face
        0.0f, 0.0f, 1.0f,texCoords[1].x + texCoords[1].w, texCoords[1].y + texCoords[1].h,
         1.0f, 0.0f, 1.0f,texCoords[1].x, texCoords[1].y + texCoords[1].h,
         1.0f,  1.0f, 1.0f,texCoords[1].x, texCoords[1].y,
        0.0f,  1.0f, 1.0f,texCoords[1].x + texCoords[1].w, texCoords[1].y,
   
        // Top face
        0.0f, 1.0f, 0.0f,texCoords[2].x, texCoords[2].y + texCoords[2].h,
        0.0f, 1.0f,  1.0f,texCoords[2].x, texCoords[2].y,
         1.0f, 1.0f,  1.0f,texCoords[2].x + texCoords[2].w, texCoords[2].y,
         1.0f, 1.0f, 0.0f,texCoords[2].x + texCoords[2].w, texCoords[2].y + texCoords[2].h,
   
        // Bottom face
        0.0f, 0.0f, 0.0f,texCoords[3].x + texCoords[3].w, texCoords[3].y + texCoords[3].h,
         1.0f, 0.0f, 0.0f,texCoords[3].x, texCoords[3].y + texCoords[3].h,
         1.0f, 0.0f,  1.0f,texCoords[3].x, texCoords[3].y,
        0.0f, 0.0f,  1.0f,texCoords[3].x + texCoords[3].w, texCoords[3].y,
   
        // Left face
        0.0f, 0.0f,  1.0f,texCoords[4].x, texCoords[4].y + texCoords[4].h,
        0.0f,  1.0f,  1.0f,texCoords[4].x, texCoords[4].y,
        0.0f,  1.0f, 0.0f,texCoords[4].x + texCoords[4].w, texCoords[4].y,
        0.0f, 0.0f, 0.0f,texCoords[4].x + texCoords[4].w, texCoords[4].y + texCoords[4].h,
   
        // Right face
        1.0f, 0.0f, 0.0f,texCoords[5].x, texCoords[5].y + texCoords[5].h,
        1.0f,  1.0f, 0.0f,texCoords[5].x, texCoords[5].y,
        1.0f,  1.0f,  1.0f,texCoords[5].x + texCoords[5].w, texCoords[5].y,
        1.0f, 0.0f,  1.0f,texCoords[5].x + texCoords[5].w, texCoords[5].y + texCoords[5].h
   };

   for(int i = 0; i < 120; i++){
    vertices[i] = vertice_[i];
   }
}