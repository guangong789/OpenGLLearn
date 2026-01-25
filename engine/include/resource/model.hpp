#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <render/shader.hpp>
#include <graphics/mesh.hpp>
#include <render/material.hpp>

class Model {
public:
    explicit Model(const std::string& path);

    void draw(Shader& shader) const;

private:
    struct SubMesh {
        Mesh mesh;
        Material material;
    };

    std::vector<SubMesh> meshes;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    SubMesh processMesh(aiMesh* mesh, const aiScene* scene);
};