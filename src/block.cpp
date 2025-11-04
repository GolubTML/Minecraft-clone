#include "../headers/block.h"
#include "../headers/shader.h"
#include "../headers/texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Block::Block(bool solid, bool visible) : isSolid(solid), isVisible(visible)
{
    if (isSolid && isVisible)
    {
        texture = nullptr;
        width = height = 0;
        position = glm::vec3(0.f, 0.f, 0.f);
    }
}

Block::Block(glm::vec3 pos, Texture* tex, float w, float h, float l, bool debug) : position(pos), texture(tex), width(w), height(h), lenght(l), debugMode(debug)
{
    isVisible = true;
    isSolid = true;
    
    float vertices[] =
    {
        // X            Y               Z            R     G     B     U    V
        -width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
         width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
         width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
        -width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

        // front face
        -width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
         width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
         width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
        -width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

        // left face
        -width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
        -width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
        -width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
        -width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

        // right face
         width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
         width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
         width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
         width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

        // bottom face
        -width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
         width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
         width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
        -width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 1.0f,

        // top face
        -width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 0.0f,
         width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 0.0f,
         width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  1.0f, 1.0f,
        -width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  0.0f, 1.0f
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

    if (texture != nullptr)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
    else
    {
        VBO = 0;
        VAO = 0;
        EBO = 0;
    }
}

void Block::draw(Shader& shader)
{
    if (!isVisible) return;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    if (texture != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        texture->bind();

        glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);
    }

    glBindVertexArray(VAO);
    
    if (debugMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    else
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (texture != nullptr)
        texture->unBind();
}