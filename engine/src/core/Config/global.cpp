#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/Config/global.hpp>

// Time
float deltaTime = 0.0f;
float lastTime  = 0.0f;

// Camera
Camera myCamera(
    glm::vec3(0.f, 0.f, 3.f),
    glm::vec3(0.f, 1.f, 0.f),
    YAW,
    PITCH
);

// Tmp
float lastX = scrWidth * 0.5f;
float lastY = scrHeight * 0.5f;
bool firstMouse = true;

// Window
GLFWwindow* window = nullptr;