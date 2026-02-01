#include <render/OutlinePass.hpp>

OutlinePass::OutlinePass(Shader& s, float sc) : shader(s), scaled(sc) {}

void OutlinePass::submit(Renderable* obj) {
    if (obj->enableOutline) outlined.emplace_back(obj);
}

void OutlinePass::render(const RenderContext& rct) {
    if (outlined.empty()) return;

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    shader.use();
    shader.set("view", rct.view);
    shader.set("projection", rct.projection);

    for (auto* obj : outlined) obj->drawOutline(shader, scaled);

    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);

    outlined.clear();
}