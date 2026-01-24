#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/Utils/stb_image.h>
#include <core/Utils/uti.hpp>
#include <core/Config/global.hpp>
#include <render/shader.hpp>
#include <core/camera.hpp>
#include <cube.hpp>
#include <scene/Lighting/light.hpp>
#include <render/material.hpp>
#include <scene/renderable.hpp>
#include <scene/Lighting/LightManager.hpp>

// extern "C" {
// __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
// }

int main() {
    window = InitWindow(scrWidth, scrHeight, "Main");
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //----------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    //----------------------------------------------------------------------
    Shader objectShader("assets/shaders/exp1/object.vs", "assets/shaders/exp1/object.fs");
    Shader lightingShader("assets/shaders/exp1/light.vs", "assets/shaders/exp1/light.fs");
    Mesh* cubeMesh = Cube::CreateCubeMesh();
    Material boxMat;
    boxMat.diffuse = LoadTextureFromFile("assets/textures/container.png");
    boxMat.specular = LoadTextureFromFile("assets/textures/container_specular.png");
    //----------------------------------------------------------------------
    LightManager lightmanager;
    std::vector<glm::vec3> pointlightPos{
        {2.0f, 1.0f, 2.0f},
        {-2.0f, 1.0f, -2.0f}
    };
    for (auto& pos : pointlightPos) lightmanager.pointlights.emplace_back(pos);
    lightmanager.spotlight.position = myCamera.Position;
    lightmanager.spotlight.direction = myCamera.Front;
    //----------------------------------------------------------------------
    objectShader.use();
    objectShader.set("material.shininess", boxMat.shininess);
    objectShader.set("hasSpecularMap", true);
    objectShader.set("ambientStrength", 0.3f);
    objectShader.set("diffuseStrength", 0.8f);
    //----------------------------------------------------------------------
    Renderable box;
    box.mesh = cubeMesh;
    box.material = &boxMat;
    box.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    box.transform.scale = glm::vec3(1.0f);
    //----------------------------------------------------------------------
    Renderable lightVisual;
    lightVisual.mesh = cubeMesh;
    lightVisual.transform.scale = glm::vec3(0.2f);
    //----------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        lightmanager.spotlight.followCamera(myCamera);

        glm::mat4 view = myCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(myCamera.Zoom),
            (float)scrWidth / (float)scrHeight,
            0.1f, 500.f
        );

        objectShader.use();
        lightmanager.upload(objectShader);
        objectShader.set("view", view);
        objectShader.set("projection", projection);
        objectShader.set("viewPos", myCamera.Position);
        box.draw(objectShader);

        lightingShader.use();
        lightingShader.set("view", view);
        lightingShader.set("projection", projection);
        for (const auto& pl : lightmanager.pointlights) {
            lightVisual.transform.position = pl.position;
            lightingShader.set("model", lightVisual.transform.matrix());
            cubeMesh->draw();
        }

        // CHECK_GL(void(0));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}