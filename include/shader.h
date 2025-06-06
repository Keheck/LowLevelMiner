#pragma once

#include <glad/glad.h>
#include <stddef.h>
#include <glm/glm.hpp>

class Shader;

class Shader {
    public:
        Shader(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file = NULL);
        ~Shader();
        void use_shader();
        int getUniformLocation(const char* uniformName);

        void setInt(const char *location, int value);
        void setVec2i(const char *location, int x, int y);
        void setVec3i(const char *location, int x, int y, int z);
        void setVec4i(const char *location, int x, int y, int z, int w);

        void setFloat(const char *location, float value);
        void setVec2f(const char *location, float x, float y);
        void setVec3f(const char *location, float x, float y, float z);
        void setVec4f(const char *location, float x, float y, float z, float w);

        void setMat2f(const char *location, glm::mat2 mat);
        void setMat3f(const char *location, glm::mat3 mat);
        void setMat4f(const char *location, glm::mat4 mat);
    private:
        unsigned int programID;

};