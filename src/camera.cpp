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

void Camera::updateFrustum(const glm::mat4& proj, const glm::mat4& view)
{
    glm::mat4 clip = proj * view;

    // Right
    frustum[0].normal.x = clip[0][3] - clip[0][0];
    frustum[0].normal.y = clip[1][3] - clip[1][0];
    frustum[0].normal.z = clip[2][3] - clip[2][0];
    frustum[0].distance = clip[3][3] - clip[3][0];

    // Left
    frustum[1].normal.x = clip[0][3] + clip[0][0];
    frustum[1].normal.y = clip[1][3] + clip[1][0];
    frustum[1].normal.z = clip[2][3] + clip[2][0];
    frustum[1].distance = clip[3][3] + clip[3][0];

    // Top
    frustum[2].normal.x = clip[0][3] - clip[0][1];
    frustum[2].normal.y = clip[1][3] - clip[1][1];
    frustum[2].normal.z = clip[2][3] - clip[2][1];
    frustum[2].distance = clip[3][3] - clip[3][1];

    // Bottom
    frustum[3].normal.x = clip[0][3] + clip[0][1];
    frustum[3].normal.y = clip[1][3] + clip[1][1];
    frustum[3].normal.z = clip[2][3] + clip[2][1];
    frustum[3].distance = clip[3][3] + clip[3][1];

    // Far
    frustum[4].normal.x = clip[0][3] - clip[0][2];
    frustum[4].normal.y = clip[1][3] - clip[1][2];
    frustum[4].normal.z = clip[2][3] - clip[2][2];
    frustum[4].distance = clip[3][3] - clip[3][2];

    // Near
    frustum[5].normal.x = clip[0][3] + clip[0][2];
    frustum[5].normal.y = clip[1][3] + clip[1][2];
    frustum[5].normal.z = clip[2][3] + clip[2][2];
    frustum[5].distance = clip[3][3] + clip[3][2];
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

bool Camera::boxInFrustum(const AABB& box) const
{
    for (int i = 0; i < 6; ++i)
    {
        glm::vec3 p = box.getPositiveVertex(frustum[i].normal);

        if (glm::dot(frustum[i].normal, p) + frustum[i].distance < 0)
            return false;
    }

    return true;
}

bool Camera::isChunkVisible(const Chunk& chunk)
{
    glm::vec3 chunkMin = chunk.chunckAABB.min;
    glm::vec3 chunkMax = chunk.chunckAABB.max;

    glm::vec3 cameraMin = position - glm::vec3(30.f);
    glm::vec3 cameraMax = position + glm::vec3(30.f);

    bool x = chunkMax.x >= cameraMin.x && chunkMin.x <= cameraMax.x;
    bool y = chunkMax.y >= cameraMin.y && chunkMin.y <= cameraMax.y;
    bool z = chunkMax.z >= cameraMin.z && chunkMin.z <= cameraMax.z;

    return x && y && z;
}