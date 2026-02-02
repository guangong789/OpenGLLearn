#include <iostream>
#include <vector>
#include <cmath>
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
#include <render/GeometryPass.hpp>
#include <render/OutlinePass.hpp>
#include <scene/Lighting/light.hpp>
#include <scene/renderable.hpp>
#include <scene/Lighting/LightManager.hpp>
#include <resource/model.hpp>
#include <cube.hpp>
#include <plane.hpp>

// extern "C" {
// __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
// }

int main() {
    screen = InitWindow(scrWidth, scrHeight, "Main");
    glfwSetFramebufferSizeCallback(screen, framebuffer_size_callback);
    glfwSetCursorPosCallback(screen, mouse_callback);
    glfwSetScrollCallback(screen, scroll_callback);
    glfwSetInputMode(screen, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //----------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //----------------------------------------------------------------------
    Shader objectShader("assets/shaders/object.vs", "assets/shaders/object.fs");
    Shader lightingShader("assets/shaders/light.vs", "assets/shaders/light.fs");
    Shader SingleColorShader("assets/shaders/object.vs", "assets/shaders/SingleColor.fs");
    //----------------------------------------------------------------------
    LightManager lightmanager;
    std::vector<glm::vec3> pointlightPos{
        {2.0f, 1.0f, 2.0f},
        {-2.0f, 1.0f, 2.0f}
    };
    for (auto& pos : pointlightPos) lightmanager.pointlights.emplace_back(pos);
    lightmanager.spotlight.position = myCamera.Position;
    lightmanager.spotlight.direction = myCamera.Front;
    //----------------------------------------------------------------------
    GeometryPass geometryPass{objectShader, lightmanager};
    OutlinePass outlinePass{SingleColorShader, 1.05f};
    //----------------------------------------------------------------------
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
    //----------------------------------------------------------------------
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
    //----------------------------------------------------------------------
    while (!glfwWindowShouldClose(screen)) {
        processInput(screen);
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        lightmanager.spotlight.followCamera(myCamera);

        glm::mat4 view = myCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(myCamera.Zoom),
            (float)scrWidth / (float)scrHeight,
            0.1f, 500.f
        );

        RenderContext rct{view, projection, myCamera.Position};

        geometryPass.submit(&obj);
        geometryPass.submit(&floor);
        geometryPass.submit(&window);
        outlinePass.submit(&obj);
        geometryPass.renderOpaque(rct);
        outlinePass.render(rct);
        geometryPass.renderTransparent(rct);

        // CHECK_GL(void(0));
        glfwSwapBuffers(screen);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}