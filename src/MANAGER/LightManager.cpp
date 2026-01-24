#include <algorithm>
#include <MANAGER/LightManager.hpp>

void LightManager::upload(Shader& shader) const {
#ifndef NDEBUG
    assert(pointlights.size() <= MAX_POINT_LIGHTS && "Too many point lights for forward renderer");
#endif
    int pointCount = std::min(static_cast<int>(pointlights.size()), MAX_POINT_LIGHTS);

    shader.set("numPointLights", pointCount);

    for (int i = 0; i < pointCount; ++i) {
        pointlights[i].upload(shader, i);
    }

    dirlight.upload(shader);
    spotlight.upload(shader);
}
