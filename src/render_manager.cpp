#include "render_manager.h"

#include <stack>
#include <glm/ext/quaternion_trigonometric.hpp>

namespace transtack {

static std::stack<glm::mat4> transformStack = std::stack<glm::mat4>({glm::mat4(1.0f)});

glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);

void push_matrix() {
    transformStack.emplace(transformStack.top());
}

void pop_matrix() {
    transformStack.pop();
}

void translate(glm::vec3 translation) {
    glm::mat4 &current = transformStack.top();
    current = glm::translate(current, translation);
}

void scale(glm::vec3 scaling) {
    glm::mat4 &current = transformStack.top();
    current = glm::scale(current, scaling);
}

void rotate(float angle, glm::vec3 axis) {
    glm::mat4 &current = transformStack.top();
    current = glm::rotate(current, angle, axis);
}

void rotate(glm::quat rotation) {
    glm::mat4 &current = transformStack.top();
    current = glm::rotate(current, glm::angle(rotation), glm::axis(rotation));
}

void multiply(glm::mat4 matrix) {
    glm::mat4 &current = transformStack.top();
    current = current * matrix;
}

glm::mat4 &top() {
    return transformStack.top();
}

bool is_empty() {
    return transformStack.size() <= 1;
}

}