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