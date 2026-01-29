#include <vector>
#include <plane.hpp>

MeshData Plane::GetXZ() {
    static std::vector<Vertex> vertices = {
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f},  {0,1,0}},
        {{0.5f, 0.0f, -0.5f}, {1.0f, 0.0f},  {0,1,0}},
        {{0.5f, 0.0f,  0.5f}, {1.0f, 1.0f},  {0,1,0}},
        {{-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f},  {0,1,0}}
    };
    static std::vector<unsigned int> indices = {0,1,2, 2,3,0};

    static MeshData meshdata{vertices, indices};
    return meshdata;
}

MeshData Plane::GetXY() {
    static std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f},  {0,0,1}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f},  {0,0,1}},
        {{0.5f, 0.5f,  0.0f}, {1.0f, 1.0f},  {0,0,1}},
        {{-0.5f, 0.5f,  0.0f}, {0.0f, 1.0f},  {0,0,1}}
    };
    static std::vector<unsigned int> indices = {0,1,2, 2,3,0};

    static MeshData meshdata{vertices, indices};
    return meshdata;
}

MeshData Plane::GetYZ() {
    static std::vector<Vertex> vertices = {
        {{0.0f, -0.5f, -0.5f}, {0.0f, 0.0f},  {1,0,0}},
        {{0.0f, 0.5f, -0.5f}, {1.0f, 0.0f},  {1,0,0}},
        {{0.0f, 0.5f,  0.5f}, {1.0f, 1.0f},  {1,0,0}},
        {{0.0f, -0.5f,  0.5f}, {0.0f, 1.0f},  {1,0,0}}
    };
    static std::vector<unsigned int> indices = {0,1,2, 2,3,0};

    static MeshData meshdata{vertices, indices};
    return meshdata;
}