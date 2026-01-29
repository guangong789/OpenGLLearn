#include <render/material.hpp>

Material::Material()
    : diffuse(Texture2D::DefaultDiffusePtr()),
      specular(Texture2D::DefaultSpecularPtr()) {}

Material::Material(std::shared_ptr<const Texture2D> diff, std::shared_ptr<const Texture2D> spec)
    : diffuse(diff ? diff : Texture2D::DefaultDiffusePtr()),
      specular(spec ? spec : Texture2D::DefaultSpecularPtr()) {
        assert(diffuse);
        assert(specular);
}

void Material::bind(Shader& shader) const {
    diffuse->bind(0);
    shader.set("material.diffuse", 0);

    specular->bind(1);
    shader.set("material.specular", 1);

    shader.set("material.shininess", shininess);
    shader.set("material.diffuseStrength", diffuseStrength);
}