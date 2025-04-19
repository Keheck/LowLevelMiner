#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class Camera {
    public:
        Camera(glm::vec3 initPos, float initPitch, float initYaw);
        glm::mat4 getViewMatrix();
        void flyLocal(glm::vec3 direction);
        void walk(glm::vec3 direction);
        void rotate(float dYaw, float dPitch);
        glm::vec3 position;
        glm::vec3 forward, right, up;
        float pitch = 0;
        float yaw = 0;
};