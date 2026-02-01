#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <resource/model.hpp>
#include <core/Utils/uti.hpp>

Model::Model(const std::string& path) : myPath(path){
    loadModel(path);
}

Model::Model(std::shared_ptr<const Mesh> mesh, Material&& material) {
    meshes.emplace_back(std::move(mesh), std::move(material));
}

void Model::drawGeometry(Shader& shader) const {
    for (const auto& sub : meshes) {
        sub.material.bind(shader);
        sub.mesh->draw();
    }
}

void Model::drawMeshOnly() const {
    for (const auto& sub : meshes) sub.mesh->draw();
}

std::shared_ptr<const Texture2D> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, const std::string& directory) {
    if (mat->GetTextureCount(type) == 0) return nullptr;

    aiString str;
    mat->GetTexture(type, 0, &str);

    std::string fullPath = directory + "/" + str.C_Str();

    auto it = textureCache.find(fullPath);
    if (it != textureCache.end()) return it->second;

    auto tex = Texture2D::FromFile(fullPath);
    textureCache.emplace(fullPath, tex);
    return tex;
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

Model::SubMesh Model::processMesh(aiMesh* aimesh, const aiScene* aiscene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // vertex
    for (unsigned int i = 0; i < aimesh->mNumVertices; ++i) {
        Vertex v{};
        // position
        v.Position = {aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z};
        // normal
        if (aimesh->HasNormals()) {
            v.Normal = {aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z};
        }
        // texture
        if (aimesh->mTextureCoords[0]) {
            v.Texcoord = {aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y};
        }
        vertices.emplace_back(v);
    }
    // index
    for (unsigned int i = 0; i < aimesh->mNumFaces; ++i) {
        const aiFace& face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    // material
    Material material;

    if (aiscene->mMaterials && aimesh->mMaterialIndex < aiscene->mNumMaterials) {
        aiMaterial* mat = aiscene->mMaterials[aimesh->mMaterialIndex];

        auto diff = LoadMaterialTexture(mat, aiTextureType_DIFFUSE, directory);
        auto spec = LoadMaterialTexture(mat, aiTextureType_SPECULAR, directory);

        material.diffuse  = diff ? diff : Texture2D::DefaultDiffusePtr();
        material.specular = spec ? spec : Texture2D::DefaultSpecularPtr();

        mat->Get(AI_MATKEY_SHININESS, material.shininess);
    }

    auto mesh = std::make_shared<Mesh>(vertices, indices);
    return SubMesh{mesh, std::move(material)};
}