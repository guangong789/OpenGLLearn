#pragma once
#include <GLFW/glfw3.h>
#include <string>

#define CHECK_GL(x) \
    do { \
        _check_gl_error(#x, __FILE__, __LINE__); \
        x; \
        _check_gl_error(#x, __FILE__, __LINE__); \
    } while(0)

void _check_gl_error(const char* expr, const char* file, int line);
void checkShaderCompile(GLuint shader, const std::string &name);
void checkProgramLink(GLuint program, const std::string &name);
GLFWwindow* InitWindow(int width, int height, const char* title);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int LoadTextureFromFile(const char* path);