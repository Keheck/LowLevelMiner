#include "shader.h"
#include <stdexcept>
#include <iostream>

static void log_shader_state(unsigned int shaderID) {
    int logLen;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
    char log[logLen];
    glGetShaderInfoLog(shaderID, logLen, NULL, log);
    std::cout << log << std::endl;
}

static void compile_shader(unsigned int shaderID, const char **shader_source, int *result) {
    glShaderSource(shaderID, 1, shader_source, NULL);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, result);
}

Shader::Shader(const char **vertex_shader_source, const char **fragment_shader_source, const char **geometry_shader_source) {
    if(vertex_shader_source == NULL) {
        throw std::invalid_argument("vertex shader may not be null");
    }

    if(fragment_shader_source == NULL) {
        throw std::invalid_argument("fragment shader may not be null");
    }

    int success = 0;
    int logLen = 0;

    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int geometry_shader_id = geometry_shader_source ? glCreateShader(GL_GEOMETRY_SHADER) : 0;

    // =========================
    // = COMPILE VERTEX SHADER =
    // =========================
    compile_shader(vertex_shader_id, vertex_shader_source, &success);
    if(!success) {
        log_shader_state(vertex_shader_id);
        goto delShaders;
    }

    // ===========================
    // = COMPILE FRAGMENT SHADER =
    // ===========================

    compile_shader(fragment_shader_id, fragment_shader_source, &success);
    if(!success) {
        log_shader_state(fragment_shader_id);
        goto delShaders;
    }

    if(!geometry_shader_source) {
        goto skipGeometryShader;
    }

    // ===========================
    // = COMPILE GEOMETRY SHADER =
    // ===========================
    compile_shader(geometry_shader_id, geometry_shader_source, &success);
    if(!success) {
        log_shader_state(geometry_shader_id);
        goto delShaders;
    }
    
    // =======================
    // = LINK SHADER PROGRAM =
    // =======================
    skipGeometryShader:
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex_shader_id);
    glAttachShader(this->programID, fragment_shader_id);
    if(geometry_shader_source) glAttachShader(this->programID, geometry_shader_id);

    glLinkProgram(this->programID);
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramiv(this->programID, GL_INFO_LOG_LENGTH, &logLen);
        char log[logLen];
        glGetProgramInfoLog(this->programID, logLen, NULL, log);
        std::cout << log << std::endl;
        glDeleteProgram(this->programID);
        goto delShaders;
    }
    
    delShaders:
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    if(geometry_shader_source) glDeleteShader(geometry_shader_id);

    if(!success) {
        throw std::runtime_error("Shader compilation failed!");
    }

    return;
}

void Shader::cleanup() {
    if(this->programID) (this->programID);
    this->programID = 0;
}

Shader::~Shader() {
    this->cleanup();
}

void Shader::use_shader() {
    glUseProgram(this->programID);
}

int Shader::getUniformLocation(const char* uniformName) {
    return glGetUniformLocation(this->programID, uniformName);
}