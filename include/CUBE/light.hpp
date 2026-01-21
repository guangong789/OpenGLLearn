#pragma once
#include <glm/glm.hpp>
#include <GRAPHICS/shader.hpp>

struct Light {
    glm::vec3 position {0.0f};
    glm::vec3 direction {0.0f, -1.0f, 0.0f};

    glm::vec3 ambient  {0.1f};
    glm::vec3 diffuse  {1.0f};
    glm::vec3 specular {1.0f};

    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    int type{0}; // 0=dir,1=point,2=spot

    void upload(Shader& shader) const;
};