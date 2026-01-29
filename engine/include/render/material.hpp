#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <render/shader.hpp>
#include <resource/texture.hpp>

struct Material {
    std::shared_ptr<const Texture2D> diffuse;
    std::shared_ptr<const Texture2D> specular;

    float shininess{64.0f};
    float diffuseStrength{0.8f};

    Material();
    Material(std::shared_ptr<const Texture2D> diff, std::shared_ptr<const Texture2D> spec);

    void bind(Shader& shader) const;
};