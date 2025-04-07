#include "render_object.h"
#include <stdarg.h>

RenderObject::RenderObject(std::vector<float> &vertexData, std::vector<int> &indices) {
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderObject::bindArray() {
    glBindVertexArray(this->VAO);
}

void RenderObject::setVertexData(int count, const VertexDataDescriptor* descriptors) {
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

RenderObject::~RenderObject() {
    cleanup();
}

void RenderObject::cleanup() {
    if(this->VAO) glDeleteVertexArrays(1, &(this->VAO));
    if(this->EBO) glDeleteBuffers(1, &(this->EBO));
    if(this->VBO) glDeleteBuffers(1, &(this->VBO));

    this->EBO = 0;
    this->VAO = 0;
    this->VBO = 0;
}