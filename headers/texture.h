#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
    GLuint id;
    int width, height;

    Texture(const std::string& path, int w, int h);
    void bind();
    void unBind();
};