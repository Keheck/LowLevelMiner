#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "vertex/default.shader.h"
#include "fragment/default.shader.h"
#include "fragment/yellow.shader.h"

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

    float triVertData[] {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // BOTTOM LEFT
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // BOTTOM RIGHT
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // TOP
    };

    // unsigned int EBO; 
    unsigned int leftTriVBO, leftTriVAO;
    // glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &leftTriVAO);
    glGenBuffers(1, &leftTriVBO);
    
    glBindVertexArray(leftTriVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, leftTriVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVertData), triVertData, GL_STATIC_DRAW);
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    Shader *defaultShader = new Shader(&_vertex_default_shader, &_fragment_default_shader);

    defaultShader->use_shader();

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(leftTriVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &leftTriVAO);
    glDeleteBuffers(1, &leftTriVBO);
    delete defaultShader;

    glfwTerminate();
    return 0;
}
