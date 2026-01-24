#pragma once
#include <vector>
#include <cassert>
#include <render/shader.hpp>
#include <scene/Lighting/light.hpp>

struct LightManager {
    static constexpr int MAX_POINT_LIGHTS = 8;

    std::vector<PointLight> pointlights;
    DirLight dirlight;
    SpotLight spotlight;

    void upload(Shader& shader) const;
};
