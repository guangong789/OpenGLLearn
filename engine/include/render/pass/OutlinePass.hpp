#pragma once
#include <render/pass/RenderPass.hpp>
#include <scene/renderable.hpp>
#include <resource/texture.hpp>
#include <vector>

class OutlinePass : public RenderPass {
public:
    OutlinePass(int width, int height, Shader& shader);

    void submit(Renderable* obj);
    void execute(const RenderContext& rct) override;
    void setInputTextures(std::shared_ptr<Texture2D> color, std::shared_ptr<Texture2D> depth);
    std::shared_ptr<Texture2D> getOutputTexture() const;
    void resize(int width, int height);

private:
    void initFBO(int width, int height);
    void setupGLState();
    void restoreGLState();

private:
    GLuint fbo = 0;
    std::shared_ptr<Texture2D> inputColor;
    std::shared_ptr<Texture2D> inputDepth;
    std::shared_ptr<Texture2D> outputColor;

    Shader& shader;
    std::vector<Renderable*> outlined;
};
