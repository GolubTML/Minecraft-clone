#include "../headers/shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertex, const char* fragment)
{
    std::string vertexFile = openFile(vertex);
    std::string fragmentFile = openFile(fragment);

    const char* vertexCode = vertexFile.c_str();
    const char* fragmentCode = fragmentFile.c_str();

    GLuint vertexShader, fragmentShader;
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    checkCompile(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    checkCompile(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    checkLink(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::run()
{
    glUseProgram(ID);
}

std::string Shader::openFile(const char* path)
{
    std::fstream shader(path);

    if (!shader.is_open())
    {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << shader.rdbuf();

    return buffer.str();
}

void Shader::checkCompile(GLuint shader)
{
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(bufLog), nullptr, bufLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << bufLog << std::endl;
    }
}

void Shader::checkLink(GLuint program)
{
    if (!success)
    {
        glGetProgramInfoLog(program, sizeof(bufLog), nullptr, bufLog);
        std::cout << "ERROR::PROGRAM::LINK::LINKING_FAILED\n" << bufLog << std::endl;
    }
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}