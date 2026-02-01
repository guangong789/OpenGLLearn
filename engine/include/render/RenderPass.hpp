#pragma once
#include <glm/glm.hpp>

struct RenderContext {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 viewPos;
};

class RenderPass {
public:
    virtual ~RenderPass() = default;
    virtual void render(const RenderContext& rct) = 0;
};