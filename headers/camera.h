#pragma once
#include <glm/glm.hpp>
#include "world.h"

class GLFWwindow;

class Camera
{
public:
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 front    = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 up       = glm::vec3(0.f, 1.f, 0.f);
    glm::vec2 size     = glm::vec2(0.f, 0.f);

    float fov = 45.f;
    float sensivity = 0.1f;

    Camera(glm::vec3 pos, float w, float h, float f, float sens);

    glm::mat4 getCameraView() const;
    glm::mat4 getCameraProjection() const;

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    bool isChunkVisible(const Chunk& chunk);

private:
    bool firstMouse = false;
    float lastX = 0.f;
    float lastY = 0.f;

    float yaw = -90.f;
    float pitch = 0.f;
};