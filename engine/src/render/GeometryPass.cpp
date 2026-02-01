#include <render/GeometryPass.hpp>

GeometryPass::GeometryPass(Shader& s, LightManager& ls) : shader(s), lights(ls) {}

void GeometryPass::submit(Renderable* obj) {
    objects.emplace_back(obj);
}

void GeometryPass::render(const RenderContext& rct) {
    glEnable(GL_DEPTH_TEST);

    shader.use();
    lights.upload(shader);

    shader.set("view", rct.view);
    shader.set("projection", rct.projection);
    shader.set("viewPos", rct.viewPos);

    for (auto* obj : objects) {
        if (obj->enableOutline) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        } else {
            glStencilMask(0x00);
        }
        obj->drawGeometry(shader);
    }

    objects.clear();
}