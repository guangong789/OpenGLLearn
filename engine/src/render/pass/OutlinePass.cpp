#include <render/pass/OutlinePass.hpp>
#include <glm/glm.hpp>
#include <cassert>

OutlinePass::OutlinePass(int width, int height, Shader& s) : shader(s) {
    initFBO(width, height);
}

void OutlinePass::submit(Renderable* obj) {
    if (!obj || !obj->model) return;
    if (obj->enableOutline && !obj->transparent)
        outlined.emplace_back(obj);
}

void OutlinePass::setupGLState() {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
}

void OutlinePass::restoreGLState() {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void OutlinePass::initFBO(int width, int height) {
    if (fbo == 0) glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    outputColor = Texture2D::CreateColor(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputColor->getId(), 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE && "OutlinePass FBO incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OutlinePass::resize(int width, int height) {
    initFBO(width, height);
}

void OutlinePass::setInputTextures(std::shared_ptr<Texture2D> color, std::shared_ptr<Texture2D> depth) {
    inputColor = std::move(color);
    inputDepth = std::move(depth);
}

std::shared_ptr<Texture2D> OutlinePass::getOutputTexture() const {
    return outputColor;
}

void OutlinePass::execute(const RenderContext& rct) {
    if (!inputColor || !inputDepth) return;
    bool enableOutline = !outlined.empty();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, rct.width, rct.height);

    setupGLState();

    shader.use();
    inputColor->bind(0);
    inputDepth->bind(1);
    shader.set("uColor", 0);
    shader.set("uDepth", 1);
    shader.set("uTexelSize", glm::vec2(1.0f / rct.width, 1.0f / rct.height));
    shader.set("uOutlineColor", glm::vec3(0.0f, 1.0f, 1.0f));
    shader.set("uThreshold", 0.005f);
    shader.set("uThickness", 2.0f);
    shader.set("uNear", 0.1f);
    shader.set("uFar", 500.0f);
    shader.set("uEnableOutline", enableOutline ? 1 : 0);

    RenderPass::drawFullscreenQuad();

    restoreGLState();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    outlined.clear();
}
