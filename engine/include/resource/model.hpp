#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <render/shader.hpp>
#include <graphics/mesh.hpp>
#include <render/material.hpp>

// Assimp：树 + mesh池

class Model {
public:
    explicit Model(const std::string& path);
    Model(std::shared_ptr<const Mesh> mesh, Material&& material);
    
    void drawGeometry(Shader& shader) const;
    void drawMeshOnly() const;

private:
    std::string myPath;
    std::string directory;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textureCache;

    struct SubMesh {
        std::shared_ptr<const Mesh> mesh;
        Material material;

        SubMesh(std::shared_ptr<const Mesh> m, Material&& ma) : mesh(std::move(m)), material(std::move(ma)) {}

        SubMesh(const SubMesh&) = delete;
        SubMesh& operator=(const SubMesh&) = delete;
        SubMesh(SubMesh&&) noexcept = default;
        SubMesh& operator=(SubMesh&&) noexcept = default;
    };

    std::vector<SubMesh> meshes;
    std::shared_ptr<const Texture2D> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& directory);
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    SubMesh processMesh(aiMesh* mesh, const aiScene* scene);
};