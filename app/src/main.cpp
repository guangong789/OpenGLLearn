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
#include <resource/model.hpp>

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
    Shader objectShader("assets/shaders/exp/object.vs", "assets/shaders/exp/object.fs");
    Shader lightingShader("assets/shaders/exp/light.vs", "assets/shaders/exp/light.fs");
    Model backpack("assets/backpack/backpack.obj");
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
    Renderable obj;
    obj.model = &backpack;
    obj.transform.scale = glm::vec3(1.0f);
    //----------------------------------------------------------------------
    Mesh* cubeMesh = Cube::CreateCubeMesh();
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
        // object
        objectShader.use();
        lightmanager.upload(objectShader);
        objectShader.set("model", glm::mat4(1.0f));
        objectShader.set("view", view);
        objectShader.set("projection", projection);
        objectShader.set("viewPos", myCamera.Position);
        obj.draw(objectShader);
        // light
        lightingShader.use();
        lightingShader.set("view", view);
        lightingShader.set("projection", projection);
        for (const auto& pl : lightmanager.pointlights) {
            glm::mat4 lightModel(1.0f);
            lightModel = glm::translate(lightModel, pl.position);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));
            lightingShader.set("model", lightModel);
            cubeMesh->draw();
        }

        // CHECK_GL(void(0));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}