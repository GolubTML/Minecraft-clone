#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "../headers/shader.h"

float vertices[] = 
{
    // X   Y 
    -0.5f, -0.5f, 0.f,
    -0.5f,  0.5f, 0.f,
    0.5f , -0.5f, 0.f,

    -0.5f, 0.5f, 0.f,
    0.5f , 0.5f, 0.f,
    0.5f, -0.5f, 0.f
};

float quadVertices[] =
{
    // X    Y     Z     R    G   B
    0.5f,   0.5f, 0.f, 1.f, 0.2f, 0.8f,
    0.5f,  -0.5f, 0.f, 0.5f, 0.6f, 1.f,
    -0.5f, -0.5f, 0.f, 0.6f, 0.1f, 0.f,
    -0.5f,  0.5f, 0.f, 1.f, 1.f, 0.6f,
};

unsigned int indeces[] = 
{
    0, 1, 3,
    1, 2, 3
};

std::string openShaderFile(const char* path)
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

int main(int argc, char** argv)
{
    if (!glfwInit())
        std::cerr << "Failed to init GLFW" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Minecraft", nullptr, nullptr);
    if (!window)
        std::cerr << "Failed to create window";
        
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GLEW" << std::endl;
        return -1;
    }

    int success;
    char infoLog[512];

    std::string vertexShaderFile   = openShaderFile("shaders/vertex.glsl");
    std::string fragmentShaderFile = openShaderFile("shaders/fragment.glsl");

    const char* vertexCode = vertexShaderFile.c_str();
    const char* fragmentCode = fragmentShaderFile.c_str();

    Shader shaderProg("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    GLuint VBO, VAO, EBO;
    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), &indeces, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    shaderProg.run();
    
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shaderProg.setFloat("time", time);

        shaderProg.run();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); -- usefull


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}