#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

struct RenderContext {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 viewPos;
    int width;
    int height;
};

class RenderPass {
public:
    virtual ~RenderPass() = default;
    virtual void execute(const RenderContext& rct) = 0;

protected:
    static void drawFullscreenQuad();

private:
    static GLuint quadVAO;
    static GLuint quadVBO;
};