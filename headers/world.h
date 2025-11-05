#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "block.h"

constexpr int CHUNK_SIZE_X = 16;
constexpr int CHUNK_SIZE_Y = 64;
constexpr int CHUNK_SIZE_Z = 16;

class Texture;
class Camera;

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

struct Chunk
{
    glm::ivec3 position;
    std::vector<Block> blocks;
    AABB chunckAABB;

    Chunk();
    Chunk(glm::ivec3 pos);

    Block& getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, const Block& b);
};

struct ivec3_hash 
{
    std::size_t operator()(const glm::ivec3& v) const noexcept 
    {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1) ^ (std::hash<int>()(v.z) << 2);
    }
};

class World
{
public:
    Texture* blocksTexture;
    std::unordered_map<glm::ivec3, Chunk, ivec3_hash> chunks;
    unsigned int seed;

    World(unsigned int seed, Texture* tex);

    void generateChunk(const glm::ivec3& pos);
    //Chunk* getChunk(const glm::ivec3& pos);
    void generateWorld(int countX, int countZ);

    void draw(Shader& shader, Camera& cam);
};