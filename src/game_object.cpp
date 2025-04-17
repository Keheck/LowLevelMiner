#include "game_object.h"

#include <cmath>

#include "render_manager.h"

void GameObject::translate(glm::vec3 translation) {
    this->mTransform.mPosition += translation;
}

void GameObject::rotate(float angle, glm::vec3 axis) {
    this->mTransform.mRotation *= glm::angleAxis(angle, glm::normalize(axis));
}

void GameObject::scale(glm::vec3 scale) {
    this->mTransform.mScale *= scale;
}

void GameObject::draw(Shader &shader) {
    transtack::push_matrix();
    shader.use_shader();

    transtack::translate(this->mTransform.mPosition);
    transtack::rotate(this->mTransform.mRotation);
    transtack::scale(this->mTransform.mScale);

    shader.setMat4f("model", transtack::top());
    shader.setMat4f("view", transtack::viewMatrix);
    shader.setMat4f("projection", transtack::projectionMatrix);
    this->mMesh.draw(shader);

    transtack::pop_matrix();
}

void GameObject::setTexture(std::string textureName, Texture texture) {
    this->mMesh.textures[textureName] = texture;
}
