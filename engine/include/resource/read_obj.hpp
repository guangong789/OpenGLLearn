#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct ObjVertex {
    glm::vec3 pos;  // 位置
    glm::vec2 uv;  // 表面参数
    glm::vec3 normal;  // 微分信息
};

bool read_obj(
    const std::string& path,
    std::vector<ObjVertex>& outVertices,
    std::vector<unsigned int>& outIndices
);