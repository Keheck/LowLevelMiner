#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "vertex/default.shader.h"
#include "fragment/default.shader.h"
#include "stb_image.h"

#define TOP_LEFT (-0.5f, 0.5f, 0.0f)
#define TOP_RIGHT (0.5f, 0.5f, 0.0f)
#define BOTTOM_LEFT (-0.5f, -0.5f, 0.0f)
#define BOTTOM_RIGHT (0.5f, -0.5f, 0.0f)


void resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int, char**){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    stbi_set_flip_vertically_on_load(1);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    float vertexData[] {
        // POSITION         // COLOR          // TEXTURE COORD
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP RIGHT
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // BOTTOM RIGHT
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // TOP LEFT
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f  // BOTTOM LEFT
    };

    unsigned int indices[] {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int width, height, nrChannels;
    unsigned int container, face;
    unsigned char *data;

    glGenTextures(1, &container);
    glGenTextures(1, &face);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container);
    
    data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to read texture file" << std::endl;
    }
    stbi_image_free(data);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, face);
    data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to read texture file" << std::endl;
    }
    stbi_image_free(data);

    Shader *defaultShader = new Shader(&_vertex_default_shader, &_fragment_default_shader);

    defaultShader->use_shader();
    glUniform1i(defaultShader->getUniformLocation("tex"), 0);
    glUniform1i(defaultShader->getUniformLocation("face"), 1);

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete defaultShader;

    glfwTerminate();
    return 0;
}
