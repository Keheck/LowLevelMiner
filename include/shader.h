#pragma once

#include <stddef.h>

class Shader;

class Shader {
    public:
        Shader(const char **vertex_shader_source, const char **fragment_shader_source, const char **geometry_shader_source = NULL);
        ~Shader();
        // void add_shader_stage(char **shader, unsigned int stage);
        void use_shader();
    private:
        unsigned int programID;

};