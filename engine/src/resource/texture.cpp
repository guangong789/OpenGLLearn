#include <string>
#include <resource/texture.hpp>
#include <core/Utils/stb_image.h>

Texture2D::Texture2D(Texture2D&& other) noexcept {
    id = other.id;
    other.id = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
    if (this == &other) return *this;
    if (id) glDeleteTextures(1, &id);
    id = other.id;
    other.id = 0;
    return *this;
}

std::shared_ptr<Texture2D> Texture2D::FromFile(const std::string& path) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if (!data) {
        glDeleteTextures(1, &tex);
        return nullptr;
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    return std::make_shared<Texture2D>(tex);
}

Texture2D::~Texture2D() {
    if (id) glDeleteTextures(1, &id);
}

void Texture2D::bind(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

static GLuint createSolidTexture(unsigned char r, unsigned char g, unsigned char b) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    unsigned char data[3] = { r, g, b };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return tex;
}

bool Texture2D::valid() const {
    return id != 0;
}

std::shared_ptr<const Texture2D> Texture2D::DefaultDiffusePtr() {
    static std::shared_ptr<Texture2D> tex =
        std::make_shared<Texture2D>(createSolidTexture(255, 255, 255));
    return tex;
}

std::shared_ptr<const Texture2D> Texture2D::DefaultSpecularPtr() {
    static std::shared_ptr<Texture2D> tex =
        std::make_shared<Texture2D>(createSolidTexture(128, 128, 128));
    return tex;
}