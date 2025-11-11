#include "../headers/world.h"
#include "../headers/stb_perlin.h"
#include "../headers/camera.h"
#include "../headers/texture.h"
#include "../headers/shader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.141592653
 
glm::vec3 AABB::getPositiveVertex(const glm::vec3& normal) const
{
    glm::vec3 v = min;
    if (normal.x >= 0) v.x = max.x;
    if (normal.y >= 0) v.y = max.y;
    if (normal.z >= 0) v.z = max.z;
    return v;
}

Chunk::Chunk() : position(0)
{
    glm::vec3 chunkMin = glm::vec3(position.x, 0, position.z);
    glm::vec3 chunkMax = chunkMin + glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);

    chunckAABB = { chunkMin, chunkMax };

    blocks.resize(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
}

Chunk::Chunk(glm::ivec3 pos, Texture* tex) : position(pos), atlas(tex) 
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

void Chunk::addFace(const glm::vec3& pos, Face face, BlockType type, unsigned int& idxOffset)
{
    if (atlas == nullptr) return;
    float uvUp[8];
    float uvSide[8];
    float uvBottom[8];

    BlockData data = Block().getBlockData(type);

    switch (face)
    {
    case Face::Top:
        atlas->getUV(data.upX, data.upY, uvUp);
        break;
    case Face::Bottom:
        atlas->getUV(data.botX, data.botY, uvBottom);
        break;
    
    default:
        atlas->getUV(data.sideX, data.sideY, uvSide);
        break;
    }

    float* uv;
    if (face == Face::Top) uv = uvUp;
    else if (face == Face::Bottom) uv = uvBottom;
    else uv = uvSide;

    const float BLOCK_SIZE = 16.f;

    glm::vec3 v[4];
    switch(face)
    {
    case Face::Top:
        v[0] = pos + glm::vec3(0,1,0);
        v[1] = pos + glm::vec3(1,1,0);
        v[2] = pos + glm::vec3(1,1,1);
        v[3] = pos + glm::vec3(0,1,1);
        break;
    case Face::Bottom:
        v[0] = pos + glm::vec3(0,0,0);
        v[1] = pos + glm::vec3(1,0,0);
        v[2] = pos + glm::vec3(1,0,1);
        v[3] = pos + glm::vec3(0,0,1);
        break;
    case Face::Front:
        v[0] = pos + glm::vec3(0,0,1);
        v[1] = pos + glm::vec3(1,0,1);
        v[2] = pos + glm::vec3(1,1,1);
        v[3] = pos + glm::vec3(0,1,1);
        break;
    case Face::Back:
        v[0] = pos + glm::vec3(0,0,0);
        v[1] = pos + glm::vec3(1,0,0);
        v[2] = pos + glm::vec3(1,1,0);
        v[3] = pos + glm::vec3(0,1,0);
        break;
    case Face::Left:
        v[0] = pos + glm::vec3(0,0,0);
        v[1] = pos + glm::vec3(0,0,1);
        v[2] = pos + glm::vec3(0,1,1);
        v[3] = pos + glm::vec3(0,1,0);
        break;
    case Face::Right:
        v[0] = pos + glm::vec3(1,0,0);
        v[1] = pos + glm::vec3(1,0,1);
        v[2] = pos + glm::vec3(1,1,1);
        v[3] = pos + glm::vec3(1,1,0);
        break;
    }

    glm::vec3 color(1.f); // white

    for(int i = 0; i < 4; i++)
    {
        vertices.push_back(v[i].x);
        vertices.push_back(v[i].y);
        vertices.push_back(v[i].z);

        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);

        vertices.push_back(uv[i*2+0]);
        vertices.push_back(uv[i*2+1]);
    }

    indices.push_back(idxOffset + 0);
    indices.push_back(idxOffset + 1);
    indices.push_back(idxOffset + 2);
    indices.push_back(idxOffset + 2);
    indices.push_back(idxOffset + 3);
    indices.push_back(idxOffset + 0);

    idxOffset += 4;
}

void Chunk::generateMesh()
{
    vertices.clear();
    indices.clear();
    unsigned int idxOffset = 0;

    auto isSolidSafe = [&](int x, int y, int z)
    {
        if (x < 0 || x >= CHUNK_SIZE_X) return false;
        if (y < 0 || y >= CHUNK_SIZE_Y) return false;
        if (z < 0 || z >= CHUNK_SIZE_Z) return false;
        return blocks[x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y].isSolid;
    };

    for (int x = 0; x < CHUNK_SIZE_X; ++x)
    for (int y = 0; y < CHUNK_SIZE_Y; ++y)
    for (int z = 0; z < CHUNK_SIZE_Z; ++z)
    {
        Block& block = blocks[x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y];
        if (!block.isSolid) continue;

        glm::vec3 pos = glm::vec3(x, y, z);

        if (!isSolidSafe(x, y+1, z)) addFace(pos, Face::Top, block.type, idxOffset);
        if (!isSolidSafe(x, y-1, z)) addFace(pos, Face::Bottom, block.type, idxOffset);
        if (!isSolidSafe(x+1, y, z)) addFace(pos, Face::Right, block.type, idxOffset);
        if (!isSolidSafe(x-1, y, z)) addFace(pos, Face::Left, block.type, idxOffset);
        if (!isSolidSafe(x, y, z+1)) addFace(pos, Face::Front, block.type, idxOffset);
        if (!isSolidSafe(x, y, z-1)) addFace(pos, Face::Back, block.type, idxOffset);
    }

    glGenVertexArrays(1, &VAO);
    // std::cout << "Vertices: " << vertices.size() << ", Indices: " << indices.size() << "\n";
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

glm::vec3 World::getNoiseDirection(glm::vec3 point, float scale, unsigned int seed)
{
    float n1 = stb_perlin_noise3_seed(point.x * scale, point.y * scale, point.z * scale, 0, 0, 0, seed);
    float n2 = stb_perlin_noise3_seed(point.x * scale + 100, point.y * scale + 100, point.z * scale + 100, 0, 0, 0, seed + 1);

    float theta = n1 * 2.f * PI;
    float phi = (n2 + 0.5f + 0.5f) * PI;

    return glm::normalize(glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
}

bool World::isItACave(float x, float y, float z, Sphere& s)
{
    float dx = x - s.center.x;
    float dy = y - s.center.y;
    float dz = z - s.center.z;

    return pow(dx, 2) + pow(dy, 2) + pow(dz, 2) <= pow(s.radius, 2);
}

World::World(unsigned int seed, Texture* tex) : seed(seed), blocksTexture(tex) { }

void World::generateChunk(const glm::ivec3& pos)
{
    Sphere s = { glm::vec3(0, 0, 0), 5.f };
    Chunk chunk(pos * glm::ivec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), blocksTexture);

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
                if (isItACave(x, y, z, s))
                {
                    type = BlockType::Air;
                }
                else if (y < intHeight - 16)
                {
                    type = BlockType::DiamondOre;
                }
                else if (y < intHeight - 4) 
                {
                    type = BlockType::Stone;
                }
                else if (y < intHeight) 
                {
                    type = BlockType::Dirt;
                }
                else if (y == intHeight) 
                {
                    type = BlockType::Grass;
                }
                else type = BlockType::Air;

                bool solid = (type != BlockType::Air);
                chunk.setBlock(x, y, z, Block(solid, solid, type));
            }
        }
    }

    chunk.generateMesh();
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

void World::draw(Shader& shader, Camera& cam, bool debug)
{
    const float BLOCK_SIZE = 16.f;
    int drawn = 0;

    for (auto& [pos, chunk] : chunks)
    {
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(chunk.position.x, 0, chunk.position.z));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        chunk.atlas->bind();
        glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);


        AABB box;
        box.min = glm::vec3(chunk.position.x, 0, chunk.position.z);
        box.max = box.min + glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);

        if (!cam.isChunkVisible(chunk) || !cam.boxInFrustum(box)) continue;
        drawn++;

        glBindVertexArray(chunk.VAO); 

        if (debug)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, chunk.indices.size(), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, chunk.indices.size(), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
    }

    std::cout << "Chunk draw: " << drawn << "\n";
}