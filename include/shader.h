#pragma once

#include <glad/glad.h>
#include <stddef.h>

class Shader;

class Shader {
    public:
        Shader(const char **vertex_shader_source, const char **fragment_shader_source, const char **geometry_shader_source = NULL);
        ~Shader();
        // void add_shader_stage(char **shader, unsigned int stage);
        void use_shader();
        int getUniformLocation(const char* uniformName);
    private:
        unsigned int programID;

};