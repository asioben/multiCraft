#include "../include/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

int initTexture(const char* path, unsigned int *texture){
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    //texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //load texture
    int width,height,channels;
    //stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path,&width,&height,&channels,0);
    if(!data){
        printf("Failed to load texture\n");
        stbi_image_free(data);
        return 1;
    }
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width,height,0,format,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return 0;
}

void destroyTexture(unsigned int *texture){
    glDeleteTextures(1,texture);
}

SDL_FRect generateSprite(int ID, SDL_Point sheet, SDL_Point sprite){
    SDL_FRect uv;
    
    int columns = (int)(sheet.x / sprite.x);

    int row = ((ID - (ID % columns)) / columns);
    int column = (ID % columns);

    float u = (column * sprite.x) / (float)sheet.x;
    float v = (row * sprite.y) / (float)sheet.y;

    vec2s pixel = {1.0f/sheet.x, 1.0f/sheet.y};

    u += pixel.x;
    v += pixel.y;

    uv.x = u;
    uv.y = v;
    uv.w = ((float)sprite.x / (float)sheet.x) - pixel.x * 0.5f;
    uv.h = ((float)sprite.y / (float)sheet.y) - pixel.y * 0.5f;

    return uv;
}