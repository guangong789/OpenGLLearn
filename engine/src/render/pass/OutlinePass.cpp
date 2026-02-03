#include <render/pass/OutlinePass.hpp>

OutlinePass::OutlinePass(Shader& s, float sc) : shader(s), scaled(sc) {}

void OutlinePass::submit(Renderable* obj) {
    if (!obj || !obj->model) return;
    if (obj->enableOutline && !obj->transparent) 
        outlined.emplace_back(obj);
}

void OutlinePass::setupGLState() {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

void OutlinePass::restoreGLState() {
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void OutlinePass::execute(const RenderContext& rct) {
    if (outlined.empty()) return;

    setupGLState();

    shader.use();
    shader.set("view", rct.view);
    shader.set("projection", rct.projection);

    for (auto* obj : outlined) obj->drawOutline(shader, scaled);

    restoreGLState();
    outlined.clear();
}