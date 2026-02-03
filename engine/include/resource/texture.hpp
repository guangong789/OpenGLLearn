#pragma once
#include <memory>
#include <glad/glad.h>

enum class Usage {
    Color,
    Depth,
    DepthStencil
};

class Texture2D {
public:
    Texture2D() = default;
    explicit Texture2D(GLuint id) : id(id) {}
    ~Texture2D();

    static std::shared_ptr<Texture2D> FromFile(const std::string& path);
    static std::shared_ptr<Texture2D> CreateColor(int width, int height);
    static std::shared_ptr<Texture2D> CreateDepth(int width, int height);

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    void bind(GLuint unit) const;
    bool valid() const;
    void attachFBO() const;

    static std::shared_ptr<const Texture2D> DefaultDiffusePtr();
    static std::shared_ptr<const Texture2D> DefaultSpecularPtr();

    Usage getUsage() const { return usage; }
    GLuint getId() const { return id; }
    void setUsage(Usage u) { usage = u; }

private:
    GLuint id = 0;
    Usage usage = Usage::Color;
};
