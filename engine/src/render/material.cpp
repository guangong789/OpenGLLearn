#include <render/material.hpp>
#include <core/Utils/uti.hpp>

unsigned int Material::defaultSpecular() {
    static unsigned int tex = CreateDefaultSpecularTexture();
    return tex;
}

void Material::bind(Shader& shader) const {
    // diffuse
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    shader.set("material.diffuse", 0);
    // specular
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular ? specular : defaultSpecular());
    shader.set("material.specular", 1);

    shader.set("material.shininess", shininess);
    shader.set("material.ambientStrength", ambientStrength);
    shader.set("material.diffuseStrength", diffuseStrength);
}