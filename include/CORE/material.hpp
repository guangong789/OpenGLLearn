#pragma once
#include <glad/glad.h>
#include <GRAPHICS/shader.hpp>

struct Material {
    unsigned int diffuse = 0;
    unsigned int specular = 0;
    float shininess = 32.0f;

    void bind(Shader& shader) const;
};