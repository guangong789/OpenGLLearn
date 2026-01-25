#include <glad/glad.h>
#include <core/Utils/uti.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
#include <core/Config/global.hpp>
#include <core/Utils/stb_image.h>

void _check_gl_error(const char* expr, const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (err) {
            case GL_INVALID_ENUM:                  error = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "GL_OUT_OF_MEMORY"; break;
            default:                               error = "UNKNOWN_ERROR"; break;
        }
        std::cerr << "[OpenGL Error] " << error
                  << " | expr: " << expr
                  << " | file: " << file
                  << " | line: " << line
                  << std::endl;
    }
}

void checkShaderCompile(GLuint shader, const std::string &name) {
    GLint success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "[Shader Compile Error][" << name << "]:\n" << infoLog << std::endl;
    }
}

void checkProgramLink(GLuint program, const std::string &name) {
    GLint success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "[Program Link Error][" << name << "]:\n" << infoLog << std::endl;
    }
}

GLFWwindow* InitWindow(int width, int height, const char* title) {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window (check OpenGL version/profile hints)");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("Failed to initialize GLAD");
    glEnable(GL_FRAMEBUFFER_SRGB);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, width, height);
    return window;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    float velocity = myCamera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) myCamera.ProcessKeyboard(Camera_Movement::FORWARD, velocity);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) myCamera.ProcessKeyboard(Camera_Movement::BACKWARD, velocity);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) myCamera.ProcessKeyboard(Camera_Movement::LEFT, velocity);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) myCamera.ProcessKeyboard(Camera_Movement::RIGHT, velocity);
}

void framebuffer_size_callback([[maybe_unused]]GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback([[maybe_unused]]GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    myCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]double xoffset, double yoffset) {
    myCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int LoadTextureFromFile(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int w, h, channels;
    unsigned char* data = stbi_load(path, &w, &h, &channels, 0);
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return textureID;
}