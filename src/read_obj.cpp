#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <OBJ/read_obj.hpp>

struct IndexTriple {
    int v, vt, vn;
    bool operator==(const IndexTriple& o) const {
        return v == o.v && vt == o.vt && vn == o.vn;
    }
};

struct Hash {
    size_t operator()(IndexTriple const& t) const {
        return ((t.v * 73856093) ^
                (t.vt * 19349663) ^
                (t.vn * 83492791));
    }
};

bool read_obj(
    const std::string& path,
    std::vector<ObjVertex>& outVertices,
    std::vector<unsigned int>& outIndices
) {
    std::fstream file(path);
    if (!file) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }
    // 保存OBJ原始数据
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::unordered_map<IndexTriple, unsigned int, Hash> indexMap;  // 顶点去重+EBO
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            positions.emplace_back(v);
        } else if (type == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uvs.emplace_back(uv);
        } else if (type == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.emplace_back(n);
        } else if (type == "f") {
            for (int i = 0; i < 3; ++i) {
                std::string token;
                ss >> token;

                std::stringstream fs(token);
                std::string s;
                IndexTriple idx{};

                std::getline(fs, s, '/'); idx.v = std::stoi(s) - 1;
                std::getline(fs, s, '/'); idx.vt = std::stoi(s) - 1;
                std::getline(fs, s, '/'); idx.vn = std::stoi(s) - 1;
                auto it = indexMap.find(idx);
                if (it != indexMap.end()) {
                    ObjVertex vert;
                    vert.pos = positions[idx.v];
                    vert.uv = uvs[idx.vt];
                    vert.normal = normals[idx.vn];

                    unsigned int newIndex = (unsigned int)outVertices.size();
                    outVertices.emplace_back(vert);
                    outIndices.emplace_back(newIndex);
                    indexMap[idx] = newIndex;
                } else {
                    outIndices.emplace_back(it->second);  // 复用索引
                }
            }
        }
    }
    return true;
}