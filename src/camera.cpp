#include "camera.h"

Camera::Camera(glm::vec3 initPos, float initYaw, float initPitch) {
    if(initPitch < -89) initPitch = -89;
    if(initPitch > 89) initPitch = 89;

    this->position = initPos;
    this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->rotate(initYaw, initPitch);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->position, this->position + this->forward, this->up);
}

void Camera::flyLocal(glm::vec3 direction) {
    if(glm::length(direction) < FLT_EPSILON) return;

    this->position += this->right * direction.x;
    this->position += this->up * direction.y;
    this->position += this->forward * direction.z;
}

void Camera::walk(glm::vec3 direction) {
    if(glm::length(direction) < FLT_EPSILON) return;
    
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(this->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 rotatedDirection = rotation * glm::vec4(direction, 1.0f);

    this->position += rotatedDirection;
}

void Camera::rotate(float dYaw, float dPitch) {
    this->yaw += dYaw;
    this->pitch += dPitch;

    if(this->pitch < -89.0f) this->pitch = -89.0f;
    if(this->pitch > 89.0f) this->pitch = 89.0f;

    this->forward.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->forward.y = sin(glm::radians(this->pitch));
    this->forward.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->forward = glm::normalize(this->forward);

    this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    this->up = glm::normalize(glm::cross(this->right, this->forward));
}
