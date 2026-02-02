#include <render/GeometryPass.hpp>

GeometryPass::GeometryPass(Shader& s, LightManager& ls) : shader(s), lights(ls) {}

void GeometryPass::submit(Renderable* obj) {
    if (obj->transparent) transparentObjects.emplace_back(obj);
    else opaqueObjects.emplace_back(obj);
}

void GeometryPass::renderOpaque(const RenderContext& rct) {
    glEnable(GL_DEPTH_TEST);

    shader.use();
    lights.upload(shader);
    shader.set("view", rct.view);
    shader.set("projection", rct.projection);
    shader.set("viewPos", rct.viewPos);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);  // 非透明物体要写深度

    for (auto* obj : opaqueObjects) {
        if (obj->enableOutline) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);  // 写入模板缓冲以供outlinePass使用
        } else {
            glStencilMask(0x00);  // 不需要写入模板缓冲
        }
        obj->drawGeometry(shader);
    }

    opaqueObjects.clear();
}

void GeometryPass::renderTransparent(const RenderContext& rct) {
    glEnable(GL_DEPTH_TEST);

    shader.use();
    shader.set("view", rct.view);
    shader.set("projection", rct.projection);
    shader.set("viewPos", rct.viewPos);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);  // 透明物体启用深度测试但不写深度
    glStencilMask(0x00);

    std::sort(
        transparentObjects.begin(),
        transparentObjects.end(),
        [&](Renderable* a, Renderable* b) {
            float da = glm::length(a->transform.position - rct.viewPos);
            float db = glm::length(b->transform.position - rct.viewPos);
            return da > db;
        }
    );  // 由远到近排序

    for (auto* obj : transparentObjects) obj->drawGeometry(shader);

    glStencilMask(0xFF);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    transparentObjects.clear();
}