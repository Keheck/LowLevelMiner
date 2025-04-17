#pragma once

#include <vector>
#include <glad/glad.h>

class Mesh;
struct VertexDataDescriptor;

class Mesh {
    public:
        Mesh(std::vector<float> &data, std::vector<unsigned int> &indices);
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