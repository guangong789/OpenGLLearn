#include <render/OutlinePass.hpp>

OutlinePass::OutlinePass(Shader& s, float sc) : shader(s), scaled(sc) {}

void OutlinePass::submit(Renderable* obj) {
    if (obj->enableOutline && !obj->transparent) outlined.emplace_back(obj);
}

void OutlinePass::render(const RenderContext& rct) {
    if (outlined.empty()) return;

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);

    shader.use();
    shader.set("view", rct.view);
    shader.set("projection", rct.projection);

    for (auto* obj : outlined) obj->drawOutline(shader, scaled);

    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);

    outlined.clear();
}