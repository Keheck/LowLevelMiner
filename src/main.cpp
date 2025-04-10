#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "vertex/default.shader.h"
#include "vertex/light.shader.h"
#include "fragment/default.shader.h"
#include "fragment/mix.shader.h"
#include "fragment/lit.shader.h"
#include "fragment/light.shader.h"
#include "stb_image.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"

double lastTime, deltaTime;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 0.0f, 0.0f);

void resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow *window, double xPos, double yPos) {
    static float lastX = 400;
    static float lastY = 300;

    float dX = xPos - lastX;
    float dY = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 9.0f;
    dX *= sensitivity * deltaTime;
    dY *= sensitivity * deltaTime;

    camera.rotate(dX, dY);
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    const float cameraSpeed = 3.0f; // adjust accordingly

    glm::vec3 direction(0.0f, 0.0f, 0.0f);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction.z = -cameraSpeed * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction.z = cameraSpeed * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction.x = -cameraSpeed * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction.x = cameraSpeed * deltaTime;
    
    camera.flyLocal(direction);
}


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int main(int, char**){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    stbi_set_flip_vertically_on_load(1);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);

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

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const float inverseSqrt3 = 1.0f / sqrt(3.0f);
    
    std::vector<float> cubeVertices = {
        // PSOITION              UV COORDS   NORMALS
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,

        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
    
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
    };
    
    std::vector<float> lightVertices = {
        // PSOITION            
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
    
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
    
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    // glm::vec3 cubePositions[] = {
    //     glm::vec3( 0.0f,  0.0f,  0.0f),
    //     glm::vec3( 2.0f,  5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f),
    //     glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3( 2.4f, -0.4f, -3.5f),
    //     glm::vec3(-1.7f,  3.0f, -7.5f),
    //     glm::vec3( 1.3f, -2.0f, -2.5f),
    //     glm::vec3( 1.5f,  2.0f, -2.5f),
    //     glm::vec3( 1.5f,  0.2f, -1.5f),
    //     glm::vec3(-1.3f,  1.0f, -1.5f)
    // };

    Mesh cube = Mesh(cubeVertices, indices);
    cube.setVertexData(3, POS_UV_NORMAL);

    Mesh light = Mesh(lightVertices, indices);
    light.setVertexData(1, POS_UV_NORMAL);

    glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, -2.0f);
    glm::vec3 lightScale = glm::vec3(0.2f);

    Texture container("assets/textures/container.jpg");
    Texture face("assets/textures/awesomeface.png");
    
    container.occupyUnit(GL_TEXTURE0);
    face.occupyUnit(GL_TEXTURE1);
    
    Shader litShader = Shader(&_vertex_default_shader, &_fragment_lit_shader);
    Shader lightShader = Shader(&_vertex_light_shader, &_fragment_light_shader);
    
    litShader.use_shader();
    litShader.setInt("tex", 0);
    litShader.setInt("face", 1);
    litShader.setVec3f("objectColor", 1.0f, 0.5f, 0.31f);
    litShader.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
    litShader.setVec3f("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);

    glEnable(GL_DEPTH_TEST);
    const float radius = 10.0f;
    
    glm::mat4 cubeTranslation = glm::translate(glm::mat4(1.0f), cubePosition);
    glm::mat4 lightTranslation = glm::scale(glm::translate(glm::mat4(1.0f), lightPosition), lightScale);
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    
    while(!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        process_input(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);
        
        cube.bindArray();
        litShader.use_shader();
        litShader.setMat4f("projection", projection);
        litShader.setMat4f("view", view);
        litShader.setMat4f("model", cubeTranslation);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        light.bindArray();

        lightShader.use_shader();
        lightShader.setMat4f("projection", projection);
        lightShader.setMat4f("view", view);
        lightShader.setMat4f("model", lightTranslation);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // for(unsigned int i = 0; i < sizeof(cubePositions)/sizeof(cubePositions[0]); i++) {
        //     glm::vec3 position = cubePositions[i];
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, position);

        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        //     litShader.setMat4f("model", model);

        //     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // }

        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.cleanup();
    light.cleanup();
    lightShader.cleanup();
    litShader.cleanup();

    glfwTerminate();
    return 0;
}
