#include <glad/glad.h>
#include <graphics/mesh.hpp>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    setup(vertices, indices);
    indexCount = static_cast<unsigned int>(indices.size());
}

Mesh::~Mesh() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}
// 移动构造
Mesh::Mesh(Mesh&& other) noexcept {
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    indexCount = other.indexCount;

    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.indexCount = 0;
}
// 移动赋值
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this == &other) return *this;

    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    indexCount = other.indexCount;

    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.indexCount = 0;

    return *this;
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setup(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    indexCount = indices.size();
    // Generate
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Binding & Copy
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // texcoord
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Texcoord));
    // normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}