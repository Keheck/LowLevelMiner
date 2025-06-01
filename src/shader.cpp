#include "shader.h"
#include <stdexcept>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

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

Shader::Shader(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file) {
    int success = 0;
    int logLen = 0;

    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int geometry_shader_id = geometry_shader_file ? glCreateShader(GL_GEOMETRY_SHADER) : 0;
    std::ifstream shader_file;
    std::stringstream shader_file_stream;
    std::string source_code;
    const char *source_code_ptr;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // =========================
    // = COMPILE VERTEX SHADER =
    // =========================
    shader_file.open(vertex_shader_file);
    shader_file_stream << shader_file.rdbuf();
    source_code = shader_file_stream.str();
    source_code_ptr = source_code.c_str();
    shader_file_stream.str("");

    compile_shader(vertex_shader_id, &source_code_ptr, &success);
    if(!success) {
        std::cout << "Failed compiling " << vertex_shader_file << std::endl;
        log_shader_state(vertex_shader_id);
        goto delShaders;
    }

    shader_file.close();

    // ===========================
    // = COMPILE FRAGMENT SHADER =
    // ===========================
    shader_file.open(fragment_shader_file);
    shader_file_stream << shader_file.rdbuf();
    source_code = shader_file_stream.str();
    source_code_ptr = source_code.c_str();
    shader_file_stream.str("");

    compile_shader(fragment_shader_id, &source_code_ptr, &success);
    if(!success) {
        std::cout << "Failed compiling " << fragment_shader_file << std::endl;
        log_shader_state(fragment_shader_id);
        goto delShaders;
    }

    shader_file.close();

    if(!geometry_shader_file) {
        goto skipGeometryShader;
    }

    // ===========================
    // = COMPILE GEOMETRY SHADER =
    // ===========================
    shader_file.open(geometry_shader_file);
    shader_file_stream << shader_file.rdbuf();
    source_code = shader_file_stream.str();
    source_code_ptr = source_code.c_str();

    compile_shader(geometry_shader_id, &source_code_ptr, &success);
    if(!success) {
        std::cout << "Failed compiling " << geometry_shader_file << std::endl;
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
    if(geometry_shader_file) glAttachShader(this->programID, geometry_shader_id);

    glLinkProgram(this->programID);
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if(!success) {
        std::cout << "Failed linking " << vertex_shader_file << " and " << fragment_shader_file << std::endl;
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
    if(geometry_shader_file) glDeleteShader(geometry_shader_id);

    if(!success) {
        throw std::runtime_error("Shader compilation failed!");
    }
}

Shader::~Shader() {
    glDeleteProgram(this->programID);
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
