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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return 0;
}