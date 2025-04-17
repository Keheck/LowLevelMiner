#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "mesh.h"
#include "shader.h"

class GameObject;
class Transform;

class Transform {
    public:
        Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
            : mPosition(position), mRotation(rotation), mScale(scale) {}
        Transform(): mPosition(glm::vec3(0.0f)), mRotation(glm::quat()), mScale(glm::vec3(1.0f)) {}
        glm::vec3 mPosition;
        glm::quat mRotation;
        glm::vec3 mScale;
};

class GameObject {
    public:
        GameObject(Mesh &mesh) : mMesh(mesh), mTransform() {}
        GameObject(Mesh &mesh, Transform initialTransform) : mMesh(mesh), mTransform(initialTransform) {}
        void translate(glm::vec3 translation);
        void rotate(float angle, glm::vec3 axis);
        void scale(glm::vec3 scale);
        void draw(Shader &shader);
        void setTexture(std::string textureName, Texture texture);
    protected:
        Mesh &mMesh;
        Transform mTransform;
};
