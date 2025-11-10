#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>

#include "../headers/stb_image.h"
#include "../headers/shader.h"
#include "../headers/texture.h"
#include "../headers/camera.h"
#include "../headers/block.h"
#include "../headers/world.h"
#include "../headers/debugWindow.h"

unsigned int getRandomSeed() 
{
    using namespace std::chrono;
    return static_cast<unsigned int>(
        duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()
    );
}

void input(GLFWwindow* window, glm::vec3& pos, glm::vec3& front, glm::vec3& up)
{
    const float speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos += speed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos -= speed * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos -= glm::normalize(glm::cross(front, up)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos += glm::normalize(glm::cross(front, up)) * speed;
}

int main(int argc, char** argv)
{
    if (!glfwInit())
        std::cerr << "Failed to init GLFW" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Minecraft", nullptr, nullptr);
    if (!window)
        std::cerr << "Failed to create window";
        
    glfwMakeContextCurrent(window);

    //stbi_set_flip_vertically_on_load(true);

    Camera camera(glm::vec3(0.f, 30.f, 0.f), 800.f, 600.f, 90.f, 0.1f);

    glfwSetWindowUserPointer(window, &camera);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
    {
        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if (cam)
            cam->mouse_callback(window, xpos, ypos);
    }); // lamda in c++, wow

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GLEW" << std::endl;
        return -1;
    }

    //glfwSwapInterval(0);

    /*glEnable(GL_CULL_FACE);  
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);*/
    bool isVisible = false, last = false;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark(); 
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    
    Texture dirt("textures/blocks/blocks.png", 256, 256);
    
    World world(getRandomSeed(), &dirt);
    Shader shaderProg("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    shaderProg.run();
    
    world.generateWorld(10, 10);
    
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    
    static DebugWindow debugWindow;
    
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        int fps = (int)(1.f / deltaTime);
        std::string title = "Minecraft " + std::to_string(fps);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool now = (glfwGetKey(window, GLFW_KEY_F4) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS;
        if (now && !last) isVisible = !isVisible;
        last = now;

        if (isVisible)
            debugWindow.draw(window, camera, deltaTime);

        glfwSetWindowTitle(window, title.c_str());

        glEnable(GL_DEPTH_TEST);  

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        dirt.bind();
        glUniform1i(glGetUniformLocation(shaderProg.ID, "ourTexture"), 0);
        
        shaderProg.setFloat("time", time);

        input(window, camera.position, camera.front, camera.up);

        glm::mat4 view = glm::mat4(1.f);
        view = camera.getCameraView();
        glUniformMatrix4fv(glGetUniformLocation(shaderProg.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 proj;
        proj = camera.getCameraProjection();
        glUniformMatrix4fv(glGetUniformLocation(shaderProg.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

        camera.updateFrustum(proj, view);
        
        shaderProg.run();
        //test.draw(shaderProg);
        /*dirtBlock.draw(shaderProg);*/

        world.draw(shaderProg, camera, false);

        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}