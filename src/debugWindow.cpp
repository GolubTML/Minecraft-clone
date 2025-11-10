#include "../headers/debugWindow.h"
#include "../headers/camera.h"

void DebugWindow::draw(GLFWwindow* window, Camera& camera, float deltaTime)
{
    ImGui::Begin("Debug mode.");

    ImGui::Text("-=Camera=-");
    ImGui::Text("Position: (%.2f), (%.2f)", camera.position.x, camera.position.y);
    ImGui::Separator();

    float fps = 1.f / deltaTime;
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Separator();

    ImGui::End();
}