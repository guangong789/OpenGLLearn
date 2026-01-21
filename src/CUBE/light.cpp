#include <CUBE/light.hpp>

void PointLight::upload(Shader& shader) const {
    shader.use();
    shader.set("pointlight.position", position);
    shader.set("pointlight.ambient", ambient);
    shader.set("pointlight.diffuse", diffuse);
    shader.set("pointlight.specular", specular);
}

void DirectionLight::upload(Shader& shader) const {
    shader.use();
    shader.set("directionlight.direction", direction);
    shader.set("directionlight.ambient", ambient);
    shader.set("directionlight.diffuse", diffuse);
    shader.set("directionlight.specular", specular);
}