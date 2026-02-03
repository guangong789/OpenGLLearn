#include <render/pass/GeometryPass.hpp>

GeometryPass::GeometryPass(int width, int height, Shader& shader, LightManager& lights)
    : shader(shader), lights(lights) {
    initFBO(width, height);
}

void GeometryPass::resize(int width, int height) {
    initFBO(width, height);
}

void GeometryPass::submit(Renderable* obj) {
    if (!obj || !obj->model) return;
    if (obj->transparent) transparentObjects.emplace_back(obj);
    else opaqueObjects.emplace_back(obj);
}

void GeometryPass::initFBO(int width, int height) {
    if (fbo == 0) glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    colorTexture = Texture2D::CreateColor(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getId(), 0);

    depthTexture = Texture2D::CreateDepth(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getId(), 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE && "GeometryPass FBO incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryPass::execute(const RenderContext& rct) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, rct.width, rct.height);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.set("view", rct.view);
    shader.set("projection", rct.projection);
    shader.set("viewPos", rct.viewPos);
    lights.upload(shader);

    for (auto* obj : opaqueObjects) obj->drawGeometry(shader);

    glDepthMask(GL_FALSE);
    for (auto* obj : transparentObjects) obj->drawGeometry(shader);
    glDepthMask(GL_TRUE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    opaqueObjects.clear();
    transparentObjects.clear();
}

std::shared_ptr<Texture2D> GeometryPass::getColorTexture() const {
    return colorTexture;
}

std::shared_ptr<Texture2D> GeometryPass::getDepthTexture() const {
    return depthTexture;
}

GLuint GeometryPass::GetFBO() const {
    return fbo;
}