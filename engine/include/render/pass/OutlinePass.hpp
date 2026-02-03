#pragma once
#include <render/pass/RenderPass.hpp>
#include <scene/renderable.hpp>
#include <vector>

class OutlinePass : public RenderPass {
public:
    explicit OutlinePass(Shader& shader, float scale = 1.05f);

    void submit(Renderable* obj);
    void execute(const RenderContext& rct) override;
    void setScale(float s) { scaled = s; }

private:
    Shader& shader;
    float scaled;
    std::vector<Renderable*> outlined;

    void setupGLState();
    void restoreGLState();
};
