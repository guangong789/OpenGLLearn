#pragma once
#include <glm/glm.hpp>
#include <GRAPHICS/shader.hpp>
#include <CAMERA/camera.hpp>

struct LightBase {
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};

    virtual ~LightBase() = default;
};

struct DirLight : public LightBase {
    glm::vec3 direction{-0.2f, -1.0f, -0.3f};

    DirLight() = default;
    explicit DirLight(const glm::vec3& dir) : direction(dir) {}

    void upload(Shader& shader) const;
};

struct PointLight : public LightBase {
    glm::vec3 position{0.0f};

    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    PointLight() = default;
    explicit PointLight(const glm::vec3& pos) : position(pos) {}

    void upload(Shader& shader, int index) const;
};

struct SpotLight : public LightBase {
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};

    float cutoff{glm::cos(glm::radians(12.5f))};
    float outercutoff{glm::cos(glm::radians(17.5f))};

    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    void followCamera(const Camera& cam);
    void upload(Shader& shader) const;
};