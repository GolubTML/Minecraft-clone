#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>

float vertices[] = 
{
    // X   Y 
    -0.5f, -0.5f, 0.f,
    0.f  ,  0.5f, 0.f,
    0.5f ,  -0.5f, 0.f,
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

    std::cout << "Vertex shader code:\n" << vertexShaderFile << std::endl;
    std::cout << "Fragment shader code:\n" << fragmentShaderFile << std::endl;

    const char* vertexCode = vertexShaderFile.c_str();
    const char* fragmentCode = fragmentShaderFile.c_str();

    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProg;
    shaderProg = glCreateProgram();

    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProg, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINK::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glUseProgram(shaderProg);
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProg);
    
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}