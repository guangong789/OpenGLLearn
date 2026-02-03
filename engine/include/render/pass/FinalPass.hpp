#pragma once

#include <render/pass/RenderPass.hpp>
#include <resource/texture.hpp>
#include <render/shader.hpp>

class FinalPass : public RenderPass {
public:
    FinalPass(Shader& shader);

    void setInputTexture(std::shared_ptr<Texture2D> texture);
    void execute(const RenderContext& rct) override;

private:
    Shader& shader;
    std::shared_ptr<Texture2D> inputTexture;
};