#include "mesh.h"
#include <stdarg.h>
#include <utility>

Mesh::Mesh(std::vector<Vertex> &vertexData, std::vector<unsigned int> &indices) {
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    indexCount = indices.size();
}

void Mesh::bindArray() {
    glBindVertexArray(this->VAO);
}

void Mesh::setVertexData(int count, const VertexDataDescriptor* descriptors) {
    int totalSize = 0;
    for(int i = 0; i < count; i++)
        totalSize += descriptors[i].numElements * descriptors[i].elementSize;
    
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    size_t currentOffset = 0;
    for(unsigned int i = 0; i < count; i++) {
        glVertexAttribPointer(i, descriptors[i].numElements, descriptors[i].type, GL_FALSE, totalSize, (void*)currentOffset);
        currentOffset += descriptors[i].numElements * descriptors[i].elementSize;
        glEnableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader &shader) {
    int i = 0;

    for(auto keyValuePair : this->textures) {
        keyValuePair.second.occupyUnit(GL_TEXTURE0+i);
        shader.setInt(keyValuePair.first.c_str(), i);
    }
    this->bindArray();
    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
}
