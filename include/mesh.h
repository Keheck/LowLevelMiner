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
struct VertexDataDescriptor;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
    public:
        Mesh(std::vector<float> &data, std::vector<unsigned int> &indices);
        Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        ~Mesh();
        void cleanup();
        void bindArray();
        void setVertexData(int count, const VertexDataDescriptor *descriptors);
        void draw(Shader &shader);
        std::map<std::string, Texture> textures;
    private:
        unsigned int VAO, VBO, EBO;
        unsigned int indexCount;
};

struct VertexDataDescriptor {
    unsigned int type;
    int numElements;
    int elementSize;
};

const VertexDataDescriptor POS_UV_NORMAL[] = {
    {GL_FLOAT, 3, sizeof(float)},
    {GL_FLOAT, 2, sizeof(float)},
    {GL_FLOAT, 3, sizeof(float)}
};