#pragma once
#include <glad/glad.h>
#include <render/shader.hpp>

struct Material {
    unsigned int diffuse = 0;
    unsigned int specular = 0;
    float shininess = 64.0f;
    float ambientStrength = 0.3f;
    float diffuseStrength = 0.8f;

    static unsigned int defaultSpecular();
    void bind(Shader& shader) const;
};