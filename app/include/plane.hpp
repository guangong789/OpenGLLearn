#pragma once
#include <graphics/mesh.hpp>

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class Plane {
public:
    static MeshData GetXZ();
    static MeshData GetXY();
    static MeshData GetYZ();
};