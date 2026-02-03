#pragma once
#include <render/pass/RenderPass.hpp>
#include <scene/renderable.hpp>
#include <scene/Lighting/LightManager.hpp>
#include <resource/texture.hpp>
#include <vector>

class GeometryPass : public RenderPass {
public:
    GeometryPass(int width, int height, Shader& shader, LightManager& lights);
    ~GeometryPass() override = default;

    void resize(int width, int height);

    void submit(Renderable* obj);
    void execute(const RenderContext& rct) override;

    std::shared_ptr<Texture2D> getColorTexture() const;
    std::shared_ptr<Texture2D> getDepthTexture() const;
    GLuint GetFBO() const;

private:
    void initFBO(int width, int height);

private:
    GLuint fbo = 0;
    std::shared_ptr<Texture2D> colorTexture;
    std::shared_ptr<Texture2D> depthTexture;

    Shader& shader;
    LightManager& lights;
    std::vector<Renderable*> opaqueObjects;
    std::vector<Renderable*> transparentObjects;
};