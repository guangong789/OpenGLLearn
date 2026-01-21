#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec2 Texcoord;
    glm::vec3 Normal;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void draw() const;
private:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int indexCount = 0;

    void setup(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};