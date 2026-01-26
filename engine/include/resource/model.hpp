#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <render/shader.hpp>
#include <graphics/mesh.hpp>
#include <render/material.hpp>

// Assimp：树 + mesh池

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class Model {
public:
    explicit Model(const std::string& path);

    void draw(Shader& shader) const;
private:
    std::string myPath;
    std::string directory;
    std::vector<Texture> textures_loaded;

    struct SubMesh {
        Mesh mesh;
        Material material;
    };

    std::vector<SubMesh> meshes;

    void loadModel(const std::string& path);
    Texture LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& directory);
    void processNode(aiNode* node, const aiScene* scene);
    SubMesh processMesh(aiMesh* mesh, const aiScene* scene);
    void reload();
};