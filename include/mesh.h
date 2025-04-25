#pragma once

#include <vector>
#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

class Mesh;
struct Vertex;

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

class Mesh {
    public:
        Mesh(std::vector<float> &data, std::vector<unsigned int> &indices);
        Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        ~Mesh();
        void draw(Shader &shader);
        std::map<std::string, Texture*> textures;
    private:
        unsigned int VAO, VBO, EBO;
        unsigned int indexCount;
};
