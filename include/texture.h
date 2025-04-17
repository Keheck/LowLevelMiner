#pragma once

class Texture;

class Texture {
    public:
        Texture() {}
        Texture(const char* fileName);
        void occupyUnit(int unitIndex);
    private:
        unsigned int textureID;
};