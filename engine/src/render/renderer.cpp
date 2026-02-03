#include <glad/glad.h>
#include <render/renderer.hpp>
#include <render/pass/RenderPass.hpp>
#include <render/pass/GeometryPass.hpp>
#include <render/pass/OutlinePass.hpp>
#include <render/pass/FinalPass.hpp>
#include <render/shader.hpp>
#include <scene/renderable.hpp>
#include <scene/Lighting/LightManager.hpp>

Renderer::Renderer(std::unique_ptr<GeometryPass> geo, std::unique_ptr<OutlinePass> outline, std::unique_ptr<FinalPass> final)
    : geometrypass(std::move(geo)),
      outlinepass(std::move(outline)),
      finalpass(std::move(final)) {
    setupGLState();
}

void Renderer::setupGLState() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::beginFrame() {
    renderQueue.clear();
}

void Renderer::endFrame() {}

void Renderer::submit(Renderable* obj) {
    renderQueue.emplace_back(obj);
}

void Renderer::render(const RenderContext& rct) {
    for (auto& obj : renderQueue) {
        geometrypass->submit(obj);
        if (obj->enableOutline) outlinepass->submit(obj);
    }

    geometrypass->execute(rct);
    outlinepass->execute(rct);

    finalpass->setInputTexture(geometrypass->getColorTexture());
    finalpass->execute(rct);
}