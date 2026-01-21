#pragma once
#include <glm/glm.hpp>
#include <CAMERA/camera.hpp>

// Window
struct GLFWwindow;
extern GLFWwindow* window;

// Time
extern float deltaTime;
extern float lastTime;

// Camera
extern Camera ourCamera;

// Hardware
inline constexpr int scrWidth = 1000;
inline constexpr int scrHeight = 1000;

// Tmp
extern float lastX;
extern float lastY;
extern bool firstMouse;