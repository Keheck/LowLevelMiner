#pragma once

class Texture;

class Texture {
    public:
        Texture(const char* fileName);
        void occupyUnit(int unitIndex);
    private:
        unsigned int textureID;
};