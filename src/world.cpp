#include "../headers/world.h"
#include "../headers/stb_perlin.h"
#include "../headers/camera.h"
#include <iostream>

Chunk::Chunk() : position(0)
{
    glm::vec3 chunkMin = glm::vec3(position.x, 0, position.z);
    glm::vec3 chunkMax = chunkMin + glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);

    chunckAABB = { chunkMin, chunkMax };

    blocks.resize(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
}

Chunk::Chunk(glm::ivec3 pos) : position(pos) 
{
    glm::vec3 chunkMin = glm::vec3(position.x, 0, position.z);
    glm::vec3 chunkMax = chunkMin + glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);

    chunckAABB = { chunkMin, chunkMax };

    blocks.resize(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
}

Block& Chunk::getBlock(int x, int y, int z)
{
    return blocks[x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y];
}

void Chunk::setBlock(int x, int y, int z, const Block& b) 
{
    blocks[x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y] = b;
}

World::World(unsigned int seed, Texture* tex) : seed(seed), blocksTexture(tex) { }

void World::generateChunk(const glm::ivec3& pos)
{
    Chunk chunk(pos * glm::ivec3(CHUNK_SIZE_X, 1, CHUNK_SIZE_Z));

    for (int x = 0; x < CHUNK_SIZE_X; ++x) 
    {
        for (int z = 0; z < CHUNK_SIZE_Z; ++z) 
        {
            float wx = chunk.position.x + x;
            float wz = chunk.position.z + z;

            float height = stb_perlin_noise3(wx * 0.05, 0.1f * seed, wz * 0.05f , 0, 0, 0) * 10 + 20;
            int intHeight = (int)std::round(height);

            for (int y = 0; y < CHUNK_SIZE_Y; ++y) 
            {
                glm::vec3 worldPos = glm::vec3(wx, y, wz); 

                BlockType type;
                if (y < intHeight - 4) chunk.setBlock(x, y, z, Block(worldPos, blocksTexture, 1.f, 1.f, 1.f, false, BlockType::Stone));
                else if (y < intHeight)  chunk.setBlock(x, y, z, Block(worldPos, blocksTexture, 1.f, 1.f, 1.f, false, BlockType::Dirt));
                else if (y == intHeight)  chunk.setBlock(x, y, z, Block(worldPos, blocksTexture, 1.f, 1.f, 1.f, false, BlockType::Grass));
                else if (y > intHeight) chunk.setBlock(x, y, z, Block(false, false));
            }
        }
    }

    chunks[pos] = std::move(chunk);
}

void World::generateWorld(int countX, int countZ)
{
    for (int x = 0; x < countX; ++x)
    {
        for (int z = 0; z < countZ; ++z)
        {
            glm::ivec3 chunkPos = glm::ivec3(x, 0, z);
            generateChunk(chunkPos);
        }
    }
}

void World::draw(Shader& shader, Camera& cam)
{
    int drawn = 0;
    for (auto& [pos, chunk] : chunks)
    {
        if (!cam.isChunkVisible(chunk)) continue;
        drawn++;

        for (int x = 0; x < CHUNK_SIZE_X; ++x)
        for (int y = 0; y < CHUNK_SIZE_Y; ++y)
        for (int z = 0; z < CHUNK_SIZE_Z; ++z)
        {
            Block& block = chunk.getBlock(x, y, z);
            block.draw(shader);
        }
    }

    std::cout << "Chunk draw: " << drawn << "\n";
}