#pragma once
#include <glm/glm.hpp>
#include <GRAPHICS/shader.hpp>

struct Light {
    glm::vec3 position {0.0f};
    glm::vec3 ambient  {0.1f};
    glm::vec3 diffuse  {1.0f};
    glm::vec3 specular {1.0f};

    void upload(Shader& shader) const;
};
