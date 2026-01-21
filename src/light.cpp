#include <CUBE/light.hpp>

void Light::upload(Shader& shader) const {
    shader.set("light.position", position);
    shader.set("light.ambient", ambient);
    shader.set("light.diffuse", diffuse);
    shader.set("light.specular", specular);
}