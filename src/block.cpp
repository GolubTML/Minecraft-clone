#include "../headers/block.h"
#include "../headers/shader.h"
#include "../headers/texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Block::Block() : position(glm::vec3(0, 0, 0)), textureUp(nullptr), width(0), height(0), lenght(0), debugMode(false), type(BlockType::Air) { }

Block::Block(bool solid, bool visible, BlockType type) : isSolid(solid), isVisible(visible), type(type)
{
    if (isSolid && isVisible)
    {
        textureUp = nullptr;
        width = height = 0;
        position = glm::vec3(0.f, 0.f, 0.f);
    }
}

Block::Block(glm::vec3 pos, Texture* tex, float w, float h, float l, bool debug, BlockType type) : position(pos), textureUp(tex), width(w), height(h), lenght(l), debugMode(debug), type(type)
{
    isVisible = true;
    isSolid = true;

    float uvUp[8];
    float uvSide[8];
    float uvBottom[8];

    BlockData data = getBlockData(type);

    if (textureUp != nullptr)
    {
        textureUp->getUV(data.upX, data.upY, uvUp);
        textureUp->getUV(data.sideX, data.sideY, uvSide);
        textureUp->getUV(data.botX, data.botY, uvBottom);
    }
    
    float vertices[] =
    {
        // X            Y               Z            R     G     B     U    V
        -width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[0], uvSide[1],
         width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[2], uvSide[3],
         width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[4], uvSide[5],
        -width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[6], uvSide[7],

        // front face
        -width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[0], uvSide[1],
         width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[2], uvSide[3],
         width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[4], uvSide[5],
        -width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[6], uvSide[7],

        // left face
        -width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[0], uvSide[1],
        -width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[6], uvSide[7],
        -width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[4], uvSide[5],
        -width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[2], uvSide[3],

        // right face
         width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[0], uvSide[1],
         width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[6], uvSide[7],
         width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[4], uvSide[5],
         width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvSide[2], uvSide[3],

        // bottom face
        -width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvBottom[0], uvBottom[1],
         width / 2.f, -height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvBottom[2], uvBottom[3],
         width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvBottom[4], uvBottom[5],
        -width / 2.f, -height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvBottom[6], uvBottom[7],

        // top face
        -width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvUp[0], uvUp[1],
         width / 2.f,  height / 2.f, -lenght / 2.f,  1.f, 1.f, 1.f,  uvUp[2], uvUp[3],
         width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvUp[4], uvUp[5],
        -width / 2.f,  height / 2.f,  lenght / 2.f,  1.f, 1.f, 1.f,  uvUp[6], uvUp[7]
    };

    unsigned int indeces[] = 
    {
        0,1,2, 2,0,3,       // back
        4,5,6, 6,7,4,       // front
        8,9,10, 10,11,8,    // left
        12,13,14, 14,15,12, // right
        16,17,18, 18,19,16, // bottom
        20,21,22, 22,23,20  // top
    };

    if (textureUp != nullptr)
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

    if (textureUp != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        textureUp->bind();

        glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);
    }

    glBindVertexArray(VAO);
    
    if (debugMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    if (textureUp != nullptr)
    {
        textureUp->unBind();
    }
}

BlockData Block::getBlockData(BlockType& type)
{
    uint upX, upY;
    uint sideX, sideY;
    uint botX, botY;

    switch (type)
    {
    case BlockType::Dirt:
        upX = 0; upY = 1;
        sideX = 0; sideY = 1;
        botX = 0; botY = 1;
        break;
    case BlockType::Grass:
        upX = 0; upY = 5;
        sideX = 0; sideY = 4;
        botX = 0, botY = 1;
        break;
    case BlockType::Stone:
        upX = 0, upY = 9;
        sideX = 0, sideY = 9;
        botX = 0, botY = 9;
        break;
    case BlockType::IronOre:
        upX = 0; upY = 6;
        sideX = 0; sideY = 6;
        botX = 0; botY = 6;
        break;
    case BlockType::GoldOre:
        upX = 0; upY = 2;
        sideX = 0; sideY = 2;
        botX = 0; botY = 2;
        break;
    case BlockType::DiamondOre:
        upX = 0; upY = 0;
        sideX = 0; sideY = 0;
        botX = 0; botY = 0;
        break;
    case BlockType::Sand:
        upX = 0; upY = 7;
        sideX = 0; sideY = 7;
        botX = 0; botY = 7;
        break;
    default:
        break;
    }

    return { type, upX, upY, sideX, sideY, botX, botY };
}