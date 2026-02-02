#pragma once
#include <render/RenderPass.hpp>
#include <scene/renderable.hpp>
#include <scene/Lighting/LightManager.hpp>
#include <vector>

class GeometryPass : public RenderPass {
public:
    GeometryPass(Shader& shader, LightManager& lights);

    void submit(Renderable* obj);
    void renderOpaque(const RenderContext& rct);
    void renderTransparent(const RenderContext& rct);

private:
    Shader& shader;
    LightManager& lights;
    std::vector<Renderable*> opaqueObjects;
    std::vector<Renderable*> transparentObjects;
};