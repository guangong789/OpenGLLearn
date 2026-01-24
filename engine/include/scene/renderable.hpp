#pragma once
#include <glm/glm.hpp>
#include <graphics/mesh.hpp>
#include <render/material.hpp>

struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};
    glm::vec3 rotateAxis{0.0f, 1.0f, 0.0f};
    float rotateAngle{0.0f};

    glm::mat4 matrix() const;
};

class Renderable {
public:
    Mesh* mesh = nullptr;
    Material* material = nullptr;
    Transform transform;

    void draw(Shader& shader) const;
};  