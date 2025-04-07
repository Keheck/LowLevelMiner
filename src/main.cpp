#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "vertex/default.shader.h"
#include "fragment/default.shader.h"
#include "fragment/mix.shader.h"
#include "stb_image.h"
#include "texture.h"
#include "render_object.h"

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

    
    std::vector<float> vertexData = {
        // POSITION         // TEXTURE COORD
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // TOP RIGHT
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // BOTTOM RIGHT
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // TOP LEFT
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f  // BOTTOM LEFT
    };
    
    std::vector<int> indices = {
        0, 1, 2,
        1, 3, 2
    };

    RenderObject obj = RenderObject(vertexData, indices);
    obj.setVertexData(2, POS_AND_UV);
    
    Texture container("assets/textures/container.jpg");
    Texture face("assets/textures/awesomeface.png");
    
    container.occupyUnit(GL_TEXTURE0);
    face.occupyUnit(GL_TEXTURE1);
    
    Shader *defaultShader = new Shader(&_vertex_default_shader, &_fragment_mix_shader);
    
    defaultShader->use_shader();
    glUniform1i(defaultShader->getUniformLocation("tex"), 0);
    glUniform1i(defaultShader->getUniformLocation("face"), 1);
    
    while(!glfwWindowShouldClose(window)) {
        process_input(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        glUniformMatrix4fv(defaultShader->getUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(trans));
        
        // glBindVertexArray(VAO);
        obj.bindArray();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    obj.cleanup();
    delete defaultShader;

    glfwTerminate();
    return 0;
}
