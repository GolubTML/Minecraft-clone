#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include <GL/glew.h>
#include "block.h"

constexpr int CHUNK_SIZE_X = 16;
constexpr int CHUNK_SIZE_Y = 128;
constexpr int CHUNK_SIZE_Z = 16;

class Texture;
class Camera;

enum class Face
{
    Top,
    Bottom,
    Left,
    Right,
    Front,
    Back
};

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 getPositiveVertex(const glm::vec3& normal) const;
};

class Chunk
{
public:
    GLuint VBO = 0, VAO = 0, EBO = 0;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Block> blocks;

    glm::ivec3 position;

    Texture* atlas = nullptr;
    AABB chunckAABB;

    Chunk();
    Chunk(glm::ivec3 pos, Texture* tex);

    Block& getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, const Block& b);
    void addFace(const glm::vec3& pos, Face face, BlockType type, unsigned int& idxOffset);
    void generateMesh();
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

    void draw(Shader& shader, Camera& cam, bool debug);
};