#pragma once
#include <memory>
#include <glad/glad.h>

class Texture2D {
public:
    Texture2D() = default;
    explicit Texture2D(GLuint id) : id(id) {}
    static std::shared_ptr<Texture2D> FromFile(const std::string& path);

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    ~Texture2D();

    void bind(GLuint unit) const;
    bool valid() const;

    static std::shared_ptr<const Texture2D> DefaultDiffusePtr();
    static std::shared_ptr<const Texture2D> DefaultSpecularPtr();

private:
    GLuint id = 0;
};
