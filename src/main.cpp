#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "shaders.h"
#include "stb_image.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "game_object.h"
#include "render_manager.h"

double lastTime, deltaTime;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

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
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
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
    
    std::vector<Vertex> cubeVertices = {
        {glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  0.0f,  1.0f)},

        {glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
    
        {glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
    
        {glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f, 0.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(1.0f, 1.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
    
        {glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
    
        {glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
    };

    std::vector<unsigned int> cubeIndices = {
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

    std::vector<Vertex> billboardVertices = {
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
    };

    std::vector<unsigned int> billboardIndicies = {0, 1, 2, 0, 2, 3};

    std::vector<glm::vec3> vegetation = {
        glm::vec3(2.38f, -0.50f, -2.76f),
        glm::vec3(3.17f, -0.50f, -3.10f),
        glm::vec3(-3.44f, -0.50f, -3.00f),
        glm::vec3(-0.39f, -0.50f, 0.67f),
        glm::vec3(0.64f, -0.50f, -3.41f),
        glm::vec3(3.47f, -0.50f, -1.69f),
        glm::vec3(2.53f, -0.50f, -4.23f),
        glm::vec3(-2.67f, -0.50f, -4.26f),
        glm::vec3(-3.80f, -0.50f, 3.49f),
        glm::vec3(0.96f, -0.50f, 1.04f),
    };

    std::vector<float> angles;
    srand(1);

    for(int i = 0; i < 10; i++) {
        angles.push_back((float)rand()/RAND_MAX*3);
    }

    // glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, -5.0f);
    // glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, -3.0f);
    // glm::vec3 lightScale = glm::vec3(0.2f);
    
    Texture container("assets/textures/container.jpg");
    Texture concrete("assets/textures/concrete.jpg");
    Texture marble("assets/textures/marble.jpg");
    Texture grass("assets/textures/grass.png");

    Mesh cube = Mesh(cubeVertices, cubeIndices);
    Mesh billboard = Mesh(billboardVertices, billboardIndicies);
    billboard.textures["Albedo"] = grass;
    
    GameObject box1 = GameObject(cube, Transform(glm::vec3(1.0f, -0.5f, -1.5f)));
    GameObject box2 = GameObject(cube, Transform(glm::vec3(-1.5f, -0.5f, -2.0f)));
    GameObject billboardObject = GameObject(billboard);

    // box1.setTexture("Albedo", container);
    // box2.setTexture("Albedo", container);

    GameObject floor = GameObject(cube, Transform(glm::vec3(0.0f, -1.5f, 0.0f), glm::quat(1.0f, glm::vec3(0.0f)), glm::vec3(10.0f, 1.0f, 10.0f)));
    // floor.setTexture("Albedo", concrete);
    
    Shader litShader = Shader(&_vertex_default_shader, &_fragment_lit_shader);
    Shader unlitShader = Shader(&_vertex_default_shader, &_fragment_unlit_shader);
    Shader lightShader = Shader(&_vertex_light_shader, &_fragment_light_shader);
    Shader depthVisualisation = Shader(&_vertex_default_shader, &_fragment_depth_vis_shader);
    Shader outlineShader = Shader(&_vertex_default_shader, &_fragment_outline_shader);

    outlineShader.use_shader();
    outlineShader.setVec3f("outlineColor", 0.5f, 0.3f, 0.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    
    while(!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        process_input(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);

        transtack::projectionMatrix = projection;
        transtack::viewMatrix = view;

        for(int i = 0; i < 10; i++) {
            glm::vec3 grassPosition = vegetation[i];
            float angle = angles[i];

            billboardObject.mTransform.mPosition = grassPosition;
            billboardObject.mTransform.mRotation = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            billboardObject.draw(unlitShader);
        }

        cube.textures["Albedo"] = container;
        box1.draw(unlitShader);
        box2.draw(unlitShader);

        cube.textures["Albedo"] = concrete;
        floor.draw(unlitShader);

        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
