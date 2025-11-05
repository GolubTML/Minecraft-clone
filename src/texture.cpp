#include "../headers/texture.h"
#include "../headers/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path, int w, int h) : width(w), height(h)
{
    int channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Cannot load texture: " << path << std::endl;
        return;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::getUV(float posX, float posY, float uv[8])
{
    const float tileSize = 16.0f; 

    float u1  = (posX * tileSize) / (float)width;
    float v1  = (posY * tileSize) / (float)height;
    float u2 = ((posX + 1) * tileSize) / (float)width;
    float v2 = ((posY + 1) * tileSize) / (float)height;

    //std::swap(v1, v2);
    
    uv[0] = u1; uv[1] = v2;
    uv[2] = u2; uv[3] = v2;
    uv[4] = u2; uv[5] = v1;
    uv[6] = u1; uv[7] = v1;
}