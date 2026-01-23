#include <CUBE/light.hpp>

constexpr int toInt(LightType t) {
    return static_cast<int>(t);
}

void Light::upload(Shader& shader) const {
    shader.use();
    shader.set("light.position", position);
    shader.set("light.direction", direction);
    shader.set("light.ambient", ambient);
    shader.set("light.diffuse", diffuse);
    shader.set("light.specular", specular);
    shader.set("light.cutoff", cutoff);
    shader.set("light.outercutoff", outercutoff);
    shader.set("light.constant", constant);
    shader.set("light.linear", linear);
    shader.set("light.quadratic", quadratic);
    shader.set("light.type", toInt(type));
}

void Light::followCamera(const Camera& cam) {
    position = cam.Position;
    direction = cam.Front;
}