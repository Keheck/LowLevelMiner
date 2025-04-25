#include "texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>


unsigned char missingTexture[] {
    0x00, 0x00, 0x00,    0xFF, 0x00, 0xFF, 0x00, 0x00,
    0xFF, 0x00, 0xFF,    0x00, 0x00, 0x00, 0x00, 0x00
};

const int channelsToFormat[] {
    0, GL_RED, GL_RG, GL_RGB, GL_RGBA
};

Texture::Texture(const char *fileName) {
    int width, height, numChannels;
    unsigned char *data = stbi_load(fileName, &width, &height, &numChannels, 0);

    int previousActive;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &previousActive);

    glGenTextures(1, &(this->textureID));
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channelsToFormat[numChannels], GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to read texture " << fileName << ", using missing texture instead" << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, missingTexture);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &(this->textureID));
}

void Texture::occupyUnit(int unitIndex) {
    glActiveTexture(unitIndex);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}