#include <scene/Lighting/light.hpp>

void DirLight::upload(Shader& shader) const {
    shader.set("dirlight.direction", direction);
    shader.set("dirlight.diffuse", diffuse);
    shader.set("dirlight.specular", specular);
}

void PointLight::upload(Shader& shader, int index) const {
    std::string prefix = "pointlights[" + std::to_string(index) + "].";

    shader.set(prefix + "position", position);
    shader.set(prefix + "diffuse", diffuse);
    shader.set(prefix + "specular", specular);
    shader.set(prefix + "constant", constant);
    shader.set(prefix + "linear", linear);
    shader.set(prefix + "quadratic", quadratic);
}

void SpotLight::followCamera(const Camera& cam) {
    position  = cam.Position;
    direction = cam.Front;
}

void SpotLight::upload(Shader& shader) const {
    shader.set("spotlight.position", position);
    shader.set("spotlight.direction", direction);
    shader.set("spotlight.cutoff", cutoff);
    shader.set("spotlight.outercutoff", outercutoff);
    shader.set("spotlight.diffuse", diffuse);
    shader.set("spotlight.specular", specular);
    shader.set("spotlight.constant", constant);
    shader.set("spotlight.linear", linear);
    shader.set("spotlight.quadratic", quadratic);
}