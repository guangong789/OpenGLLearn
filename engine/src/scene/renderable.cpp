#include <glm/gtc/matrix_transform.hpp>
#include <scene/renderable.hpp>

glm::mat4 Transform::matrix() const {
    glm::mat4 m{1.0f};
    m = glm::translate(m, position);
    m = glm::rotate(m, glm::radians(rotateAngle), rotateAxis);
    m = glm::scale(m, scale);
    return m;
}

void Renderable::draw(Shader& shader) const {
    shader.set("model", transform.matrix());
    model->draw(shader);
}