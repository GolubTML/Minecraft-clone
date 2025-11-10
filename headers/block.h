#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture;
class Shader;

enum class Face
{
    Top,
    Bottom,
    Left,
    Right,
    Front,
    Back
};

enum BlockType
{
    Air,
    Dirt,
    Grass,
    Stone,
    Sand,
    IronOre,
    GoldOre,
    DiamondOre
};

struct BlockData
{
    BlockType blockType;
    uint upX, upY;
    uint sideX, sideY;
    uint botX, botY;
};

class Block
{
private:
    GLuint VBO, VAO, EBO;

public:
    Texture* textureUp = nullptr;
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    BlockType type;
    float width, height, lenght;

    bool isSolid = false;
    bool isVisible = false;
    bool debugMode = false;
    
    Block();
    Block(bool solid, bool visible, BlockType type);
    
    BlockData getBlockData(BlockType& type);
};