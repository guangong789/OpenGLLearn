#include <glad/glad.h>
#include <render/pass/FinalPass.hpp>

FinalPass::FinalPass(Shader& shader) : shader(shader) {}

void FinalPass::setInputTexture(std::shared_ptr<Texture2D> texture) {
    inputTexture = std::move(texture);
}

void FinalPass::execute(const RenderContext&) {
    if (!inputTexture || !inputTexture->valid()) return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    shader.use();
    inputTexture->bind(0);
    shader.set("uColor", 0);

    RenderPass::drawFullscreenQuad();

    glEnable(GL_DEPTH_TEST);
}
