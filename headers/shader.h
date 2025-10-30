#pragma once
#include <GL/glew.h>
#include <string>

class Shader
{
public:
    GLuint ID;

    Shader(const char* vertex, const char* fragment);
    ~Shader();

    void run();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    int success;
    char bufLog[512];
    std::string openFile(const char* path);

    void checkCompile(GLuint shader);
    void checkLink(GLuint program);
};