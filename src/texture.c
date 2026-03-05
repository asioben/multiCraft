#include "../include/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

int initTexture(const char* path, unsigned int *texture){
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    //texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //load texture
    int width,height,channels;
    unsigned char *data = stbi_load(path,&width,&height,&channels,0);
    if(!data){
        printf("Failed to load texture\n");
        stbi_image_free(data);
        return 1;
    }
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return 0;
}

void destroyTexture(unsigned int *texture){
    glDeleteTextures(1,texture);
}

void generateSprite(int ID, SDL_Point sheet, SDL_Point sprite, vec2s *uv){
    int rows = (int)(sheet.x / sprite.x);
    int columns = (int)(sheet.y / sprite.y);

    int row = ((ID - (ID % rows)) / rows) +1;
    int column = (ID % rows);

    vec2s tex[4] = {
        {column * sprite.x, row * sprite.y},
        {(column * sprite.x) + sprite.x, row * sprite.y},
        {column * sprite.x, (row * sprite.y) + sprite.y},
        {(column * sprite.x) + sprite.x, (row * sprite.y) + sprite.y}
    };

    uv = tex;
}

void generateCube(float *vertices, BlockType block){
    SDL_Point sheet = {384,704};
    SDL_Point sprite = {16,16};
    float origin[] ={
    // ===== BACK FACE (-Z) =====
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    // ===== FRONT FACE (+Z) =====
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    // ===== LEFT FACE (-X) =====
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // ===== RIGHT FACE (+X) =====
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // ===== BOTTOM FACE (-Y) =====
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // ===== TOP FACE (+Y) =====
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f
};
    switch(block){
        case GRASS:{
            int spritesID[6] = {4,4,4,4,3,1};
            vec2s texCoord[6][4];
            int face = 0;
            int counter = 0;
            printf("////START");
            for(int i = 0; i < 6; i++)generateSprite(spritesID[i],sprite,sheet,texCoord[i]);
            for(int k = 0; k < 48; k++){
                if(k % 2 == 0) {
                    origin[3 + (5*(k/2))] = texCoord[face][counter].x;
                    printf("(%f,",origin[3 + (5*(k/2))]);
                }
                if(k % 2 == 1) {
                    origin[4 + (5*((k-1)/2))] = texCoord[face][counter].y;
                    printf("%f)\n",origin[4 + (5*((k-1)/2))]);
                }
                if(k > 0){
                    if(k % 8 == 0) {
                        face += 1;
                        counter = 0;
                    }
                    if(k % 2 == 0) counter += 1;
                }
            }
        }break;
        default: printf("Default texture");
    }

    vertices = origin;
    printf("END/////");
}