#include <CORE/material.hpp>

void Material::bind(Shader& shader) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    shader.set("material.diffuse", 0);

    if (specular) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular);
        shader.set("material.specular", 1);
        shader.set("hasSpecularMap", true);
    } else {
        shader.set("hasSpecularMap", false);
    }

    shader.set("material.shininess", shininess);
}