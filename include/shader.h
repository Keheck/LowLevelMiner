#pragma once

#include <glad/glad.h>
#include <stddef.h>
#include <glm/glm.hpp>

class Shader;

class Shader {
    public:
        Shader(const char **vertex_shader_source, const char **fragment_shader_source, const char **geometry_shader_source = NULL);
        ~Shader();
        void cleanup();
        void use_shader();
        int getUniformLocation(const char* uniformName);

        void setInt(const char *location, int value);
        void setVec2i(const char *location, int* vec);
        void setVec3i(const char *location, int* vec);
        void setVec4i(const char *location, int* vec);

        void setFloat(const char *location, int value);
        void setVec2f(const char *location, float* vec);
        void setVec3f(const char *location, float* vec);
        void setVec4f(const char *location, float* vec);

        void setMat2f(const char *location, glm::mat2 &mat);
        void setMat3f(const char *location, glm::mat3 &mat);
        void setMat4f(const char *location, glm::mat4 &mat);
    private:
        unsigned int programID;

};