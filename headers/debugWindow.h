#pragma once

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"

class Camera;

class DebugWindow
{
public:
    void init(GLFWwindow* window);
    void draw(GLFWwindow* window, Camera& camera, float deltaTime);
};