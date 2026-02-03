#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/camera.hpp>
#include <core/Utils/stb_image.h>
#include <core/Utils/uti.hpp>
#include <core/Config/global.hpp>

#include <render/shader.hpp>
#include <render/material.hpp>
#include <render/pass/GeometryPass.hpp>
#include <render/pass/OutlinePass.hpp>
#include <render/pass/FinalPass.hpp>
#include <render/renderer.hpp>

#include <scene/Lighting/light.hpp>
#include <scene/renderable.hpp>
#include <scene/Lighting/LightManager.hpp>
#include <resource/model.hpp>

#include <cube.hpp>
#include <plane.hpp>

int main() {
    // ------------------ Init ------------------
    screen = InitWindow(scrWidth, scrHeight, "Main");
    glfwSetFramebufferSizeCallback(screen, framebuffer_size_callback);
    glfwSetCursorPosCallback(screen, mouse_callback);
    glfwSetScrollCallback(screen, scroll_callback);
    glfwSetInputMode(screen, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ------------------ Shaders ------------------
    Shader objectShader("assets/shaders/object.vs", "assets/shaders/object.fs");
    Shader outlineShader("assets/shaders/FullScreen.vs", "assets/shaders/OutlinePost.fs");
    Shader fullscreenShader("assets/shaders/FullScreen.vs", "assets/shaders/FullScreen.fs");
    // ------------------ Lights ------------------
    LightManager lightmanager;
    std::vector<glm::vec3> pointlightPos{{2.0f, 1.0f, 2.0f}, {-2.0f, 1.0f, 2.0f}};
    for (auto& pos : pointlightPos) lightmanager.pointlights.emplace_back(pos);
    lightmanager.spotlight.position = myCamera.Position;
    lightmanager.spotlight.direction = myCamera.Front;
    // ------------------ Pass ------------------
    auto geometryPass = std::make_unique<GeometryPass>(scrWidth, scrHeight, objectShader, lightmanager);
    auto outlinePass = std::make_unique<OutlinePass>(scrWidth, scrHeight, outlineShader);
    auto finalPass = std::make_unique<FinalPass>(fullscreenShader);
    // ------------------ Renderer ------------------
    Renderer renderer(std::move(geometryPass), std::move(outlinePass), std::move(finalPass));
    // ------------------ Models ------------------
    Model backpack("assets/backpack/backpack.obj");

    auto floordata = Plane::GetXZ();
    std::shared_ptr<Mesh> floorMesh = std::make_shared<Mesh>(floordata.vertices, floordata.indices);
    Material floorMat;
    floorMat.diffuse = Texture2D::FromFile("assets/exp/floor.jpg");
    Model floorModel{floorMesh, std::move(floorMat)};

    auto windowdata = Plane::GetYZ();
    std::shared_ptr<Mesh> windowMesh = std::make_shared<Mesh>(windowdata.vertices, windowdata.indices);
    Material windowMat;
    windowMat.diffuse = Texture2D::FromFile("assets/exp/window.png");
    Model windowModel{windowMesh, std::move(windowMat)};
    // ------------------ Renderables ------------------
    Renderable obj;
    obj.model = &backpack;
    obj.enableOutline = true;

    Renderable floor;
    floor.model = &floorModel;
    floor.transform.position = {0.0f, -2.0f, 0.0f};
    floor.transform.scale = {20.0f, 1.0f, 20.0f};

    Renderable window;
    window.model = &windowModel;
    window.transform.position = {3.0f, 0.0f, 0.0f};
    window.transform.scale = {1.0f, 2.0f, 2.0f};
    window.transparent = true;
    // ------------------ Main loop ------------------
    while (!glfwWindowShouldClose(screen)) {
        processInput(screen);
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        lightmanager.spotlight.followCamera(myCamera);

        glm::mat4 view = myCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(myCamera.Zoom),
            (float)scrWidth / (float)scrHeight,
            0.1f, 500.f
        );
        RenderContext rct{view, projection, myCamera.Position, scrWidth, scrHeight};

        renderer.beginFrame();
        renderer.submit(&obj);
        renderer.submit(&floor);
        renderer.submit(&window);

        renderer.render(rct);

        renderer.endFrame();

        glfwSwapBuffers(screen);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
