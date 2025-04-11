#pragma once

#include <vector>
#include <glad/glad.h>

class Mesh;
struct VertexDataDescriptor;

class Mesh {
    public:
        Mesh(std::vector<float> &data, std::vector<int> &indices);
        ~Mesh();
        void cleanup();
        void bindArray();
        void setVertexData(int count, const VertexDataDescriptor *descriptors);
    private:
        unsigned int VAO, VBO, EBO;
        int textures[32] = {0};
};

struct VertexDataDescriptor {
    unsigned int type;
    int numElements;
    int elementSize;
};

const VertexDataDescriptor POS_AND_UV[] = {
    {GL_FLOAT, 3, sizeof(float)},
    {GL_FLOAT, 2, sizeof(float)}
};