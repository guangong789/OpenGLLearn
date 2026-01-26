#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <resource/model.hpp>
#include <core/Utils/uti.hpp>

Model::Model(const std::string& path) : myPath(path){
    loadModel(path);
}

void Model::draw(Shader& shader) const {
    for (const auto& sub : meshes) {
        sub.material.bind(shader);
        sub.mesh.draw();
    }
}

void Model::loadModel(const std::string& path) {
    myPath = path;
    size_t pos = path.find_last_of("/\\");
    directory = (pos == std::string::npos) ? "." : path.substr(0, pos);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate |
        aiProcess_GenNormals
    );
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error(std::string("Assimp error: ") + importer.GetErrorString());
    }

    processNode(scene->mRootNode, scene);
}

Texture Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& directory) {
    Texture texture{};
    texture.id = 0;
    texture.type = typeName;
    texture.path = "";

    if (mat->GetTextureCount(type) == 0) return texture;

    aiString str;
    mat->GetTexture(type, 0, &str);

    for (const auto& tex : textures_loaded) {
        if (std::strcmp(tex.path.C_Str(), str.C_Str()) == 0) return tex;
    }

    std::string filename = directory + "/" + str.C_Str();
    texture.id = LoadTextureFromFile(filename.c_str());
    texture.path = str;
    textures_loaded.emplace_back(texture);

    return texture;
}


void Model::processNode(aiNode* node, const aiScene* scene) {
    // node->mMeshes[i] 是索引，mesh在scene->mMeshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Model::SubMesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex v{};
        // position
        v.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        // normal
        if (mesh->HasNormals()) {
            v.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }
        // texture
        if (mesh->mTextureCoords[0]) {
            v.Texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }

        vertices.emplace_back(v);
    }
    // index
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    // material
    Material material;

    if (scene->mMaterials && mesh->mMaterialIndex < scene->mNumMaterials) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        Texture diff = LoadMaterialTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse", directory);
        Texture spec = LoadMaterialTexture(mat, aiTextureType_SPECULAR, "texture_specular", directory);
        material.diffuse = diff.id;
        material.specular = spec.id;

        mat->Get(AI_MATKEY_SHININESS, material.shininess);
    }

    return SubMesh{Mesh(vertices, indices),material};
}

void Model::reload() {
    meshes.clear();
    loadModel(myPath);
}