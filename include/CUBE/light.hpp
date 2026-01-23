#pragma once
#include <glm/glm.hpp>
#include <GRAPHICS/shader.hpp>
#include "global.hpp"

enum class LightType : int {
    Directional = 0,
    Point = 1,
    Spot = 2
};

constexpr int toInt(LightType t);

struct Light {
    glm::vec3 position {0.0f};
    glm::vec3 direction {0.0f, -1.0f, 0.0f};

    glm::vec3 ambient  {0.1f};
    glm::vec3 diffuse  {1.0f};
    glm::vec3 specular {1.0f};

    float cutoff{glm::cos(glm::radians(12.5f))};
    float outercutoff{glm::cos(glm::radians(17.5f))};
    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    LightType type{LightType::Directional};

    void upload(Shader& shader) const;
    void followCamera(const Camera& cam);
};