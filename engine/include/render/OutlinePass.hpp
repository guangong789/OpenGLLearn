#pragma once
#include <render/RenderPass.hpp>
#include <scene/renderable.hpp>
#include <vector>

class OutlinePass : public RenderPass {
public:
    OutlinePass(Shader& shader, float scaled = 1.05f);

    void submit(Renderable* obj);
    void render(const RenderContext& rct) override;

private:
    Shader& shader;
    float scaled;
    std::vector<Renderable*> outlined;
};