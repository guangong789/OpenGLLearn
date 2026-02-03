#include <glad/glad.h>
#include <render/pass/RenderPass.hpp>

GLuint RenderPass::quadVAO = 0;
GLuint RenderPass::quadVBO = 0;

void RenderPass::drawFullscreenQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            -1.f,  1.f,   0.f, 1.f,
            -1.f, -1.f,   0.f, 0.f,
             1.f, -1.f,   1.f, 0.f,

            -1.f,  1.f,   0.f, 1.f,
             1.f, -1.f,   1.f, 0.f,
             1.f,  1.f,   1.f, 1.f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
