#include "../headers/camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float w, float h, float f, float sens) : position(pos), size(w, h), fov(f), sensivity(sens) 
{
    lastX = size.x / 2.f;
    lastY = size.y / 2.f;
}

glm::mat4 Camera::getCameraView() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getCameraProjection() const
{
    return glm::perspective(glm::radians(fov), size.x / size.y, 0.1f, 100.f);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensivity;
    yoffset *= sensivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

bool Camera::isChunkVisible(const Chunk& chunk)
{
    glm::vec3 chunkMin = chunk.chunckAABB.min;
    glm::vec3 chunkMax = chunk.chunckAABB.max;

    glm::vec3 cameraMin = position - glm::vec3(50.f);
    glm::vec3 cameraMax = position + glm::vec3(50.f);

    bool x = chunkMax.x >= cameraMin.x && chunkMin.x <= cameraMax.x;
    bool y = chunkMax.y >= cameraMin.y && chunkMin.y <= cameraMax.y;
    bool z = chunkMax.z >= cameraMin.z && chunkMin.z <= cameraMax.z;

    return x && y && z;
}