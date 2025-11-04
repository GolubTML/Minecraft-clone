#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture;
class Shader;

class Block
{
private:
    GLuint VBO, VAO, EBO;

public:
    Texture* texture = nullptr;
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    float width, height, lenght;

    bool isSolid = false;
    bool isVisible = false;
    bool debugMode = false;
    
    Block(bool solid, bool visible);
    Block(glm::vec3 pos, Texture* tex, float w, float h, float l, bool debug);

    void draw(Shader& shader);
};