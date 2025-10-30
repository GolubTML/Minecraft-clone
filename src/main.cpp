#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "../headers/stb_image.h"
#include "../headers/shader.h"
#include "../headers/texture.h"

float quadVertices[] =
{
    // X    Y     Z     R     G     B     U    V
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

    // front face
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

    // left face
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

    // right face
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

    // bottom face
    -0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

    // top face
    -0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.f, 1.f, 1.f,  0.0f, 1.0f
};

unsigned int indeces[] = 
{
    0,1,2, 2,3,0,       // back
    4,5,6, 6,7,4,       // front
    8,9,10, 10,11,8,    // left
    12,13,14, 14,15,12, // right
    16,17,18, 18,19,16, // bottom
    20,21,22, 22,23,20  // top
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

void input(GLFWwindow* window, glm::vec3& pos, glm::vec3& front, glm::vec3& up)
{
    const float speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos += speed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos -= speed * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos -= glm::normalize(glm::cross(front, up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos += glm::normalize(glm::cross(front, up)) * speed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

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

    stbi_set_flip_vertically_on_load(true);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GLEW" << std::endl;
        return -1;
    }

    Texture dirt("textures/blocks/dirt.png", 32, 32);

    int success;
    char infoLog[512];
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  
    glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shaderProg.run();

    const float radius = 10.f;
    
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();

        glEnable(GL_DEPTH_TEST);  

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        dirt.bind();
        glUniform1i(glGetUniformLocation(shaderProg.ID, "ourTexture"), 0);
        
        shaderProg.setFloat("time", time);

        input(window, cameraPos, cameraFront, cameraUp);

        glm::mat4 model = glm::mat4(1.f);
        //model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProg.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        glm::mat4 view = glm::mat4(1.f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 proj;
        proj = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProg.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

        shaderProg.run();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); -- usefull

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}