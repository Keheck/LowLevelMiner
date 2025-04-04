#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "vertex/default.shader.h"
#include "fragment/default.shader.h"

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

    float triVerts[] {
        -0.5f, -0.5f, 0.0f, // BOTTOM LEFT
         0.5f, -0.5f, 0.0f, // BOTTOM RIGHT
         0.5f,  0.5f, 0.0f, // TOP RIGHT
        -0.5f,  0.5f, 0.0f  // TOP LEFT
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 2
    };

    unsigned int RectEBO, RectVBO, RectVAO;
    glGenBuffers(1, &RectEBO);
    glGenVertexArrays(1, &RectVAO);
    glGenBuffers(1, &RectVBO);
    
    glBindVertexArray(RectVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, RectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RectEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Shader *defaultShader = new Shader(&_vertex_default_shader, &_fragment_default_shader);

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        defaultShader->use_shader();
        glBindVertexArray(RectVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &RectVAO);
    glDeleteBuffers(1, &RectVBO);
    glDeleteBuffers(1, &RectEBO);
    delete defaultShader;

    glfwTerminate();
    return 0;
}
