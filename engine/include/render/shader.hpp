#pragma once
#include <string>
#include <cassert>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vsPath, const char* fsPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool hasUniform(const std::string& name);
    void use() const;

    template<typename T>
    void set(const std::string& name, const T& value);
private:
    std::unordered_map<std::string, GLint> uniformCache;
    GLint getUniformLocation(const std::string& name);

#ifndef NDEBUG
    static const Shader* current;
#endif
};

template<typename T>
void Shader::set(const std::string&, const T&) {
    static_assert(sizeof(T) == 0, "Unsupported uniform type");
}

// int
template<>
inline void Shader::set<int>(const std::string& name, const int& v) {
#ifndef NDEBUG
    assert(current == this && "Shader::set called but this shader is NOT bound");
#endif
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform1i(loc, v);
}

// bool
template<>
inline void Shader::set<bool>(const std::string& name, const bool& v) {
#ifndef NDEBUG
    assert(current == this && "Shader::set called but this shader is NOT bound");
#endif
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform1i(loc, (int)v);
}

// float
template<>
inline void Shader::set<float>(const std::string& name, const float& v) {
#ifndef NDEBUG
    assert(current == this && "Shader::set called but this shader is NOT bound");
#endif
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform1f(loc, v);
}

// vec2
template<>
inline void Shader::set<glm::vec2>(const std::string& name, const glm::vec2& v) {
#ifndef NDEBUG
    assert(current == this && "Shader::set called but this shader is NOT bound");
#endif
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform2fv(loc, 1, glm::value_ptr(v));
}

// vec3
template<>
inline void Shader::set<glm::vec3>(const std::string& name, const glm::vec3& v) {
#ifndef NDEBUG
    assert(current == this && "Shader::set called but this shader is NOT bound");
#endif
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform3fv(loc, 1, glm::value_ptr(v));
}

// mat4
template<>
inline void Shader::set<glm::mat4>(const std::string& name, const glm::mat4& m) {
#ifndef NDEBUG
    assert(current == this && "Shader::set called but this shader is NOT bound");
#endif
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}