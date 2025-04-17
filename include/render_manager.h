#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace transtack {
extern glm::mat4 projectionMatrix;
extern glm::mat4 viewMatrix;

void push_matrix();
void pop_matrix();

void translate(glm::vec3 translation);
void scale(glm::vec3 scaling);
void rotate(float angle, glm::vec3 axis);
void rotate(glm::quat rotation);

glm::mat4 &top();
}