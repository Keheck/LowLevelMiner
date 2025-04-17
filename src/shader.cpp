#include "shader.h"
#include <stdexcept>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

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

void Shader::setInt(const char *location, int value) {
    glUniform1i(glGetUniformLocation(this->programID, location), value);
}

void Shader::setVec2i(const char *location, int x, int y) {
    glUniform2i(glGetUniformLocation(this->programID, location), x, y);
}

void Shader::setVec3i(const char *location, int x, int y, int z) {
    glUniform3i(glGetUniformLocation(this->programID, location), x, y, z);
}

void Shader::setVec4i(const char *location, int x, int y, int z, int w) {
    glUniform4i(glGetUniformLocation(this->programID, location), x, y, z, w);
}

void Shader::setFloat(const char *location, float value) {
    glUniform1f(glGetUniformLocation(this->programID, location), value);
}

void Shader::setVec2f(const char *location, float x, float y) {
    glUniform2f(glGetUniformLocation(this->programID, location), x, y);
}

void Shader::setVec3f(const char *location, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(this->programID, location), x, y, z);
}

void Shader::setVec4f(const char *location, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(this->programID, location), x, y, z, w);
}

void Shader::setMat2f(const char *location, glm::mat2 mat) {
    glUniformMatrix2fv(glGetUniformLocation(this->programID, location), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3f(const char *location, glm::mat3 mat) {
    glUniformMatrix3fv(glGetUniformLocation(this->programID, location), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4f(const char *location, glm::mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(this->programID, location), 1, GL_FALSE, glm::value_ptr(mat));
}
