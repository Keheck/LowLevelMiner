#pragma once

class Texture;

class Texture {
    public:
        Texture() {}
        Texture(const char* fileName);
        ~Texture();
        void occupyUnit(int unitIndex);
    private:
        unsigned int textureID;
};