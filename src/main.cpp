#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
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

GLFWwindow *window;

int main(int, char**){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    stbi_set_flip_vertically_on_load(1);

    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    atexit([](){
        glfwDestroyWindow(window);
    });

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
        {glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  0.0f,  1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  0.0f,  1.0f)},

        {glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)},
    
        {glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(1.0f,  0.0f,  0.0f)},
    
        {glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f, 0.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(1.0f, 1.0f),  glm::vec3(-1.0f,  0.0f,  0.0f)},
    
        {glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  1.0f,  0.0f)},
    
        {glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(0.0f, 0.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f, -1.0f,  0.0f)},
    };

    std::vector<unsigned int> cubeIndices = {
        0, 1, 2,
        1, 3, 2,

        4, 5, 6,
        5, 7, 6,

        8, 9, 10,
        9, 11, 10,

        12, 13, 14,
        13, 15, 14,

        16, 17, 18,
        17, 19, 18,

        20, 21, 22,
        21, 23, 22
    };

    std::vector<Vertex> billboardVertices = {
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},

        {glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
    };

    std::vector<unsigned int> billboardIndicies = {
        0, 1, 2,
        1, 3, 2,

        4, 5, 6,
        5, 7, 6
    };

    std::vector<Transform> windowTransforms = {
        {glm::vec3(2.38f, -0.50f, -2.76f)},
        {glm::vec3(3.17f, -0.50f, -3.10f)},
        {glm::vec3(-3.44f, -0.50f, -3.00f)},
        {glm::vec3(-0.39f, -0.50f, 0.67f)},
        {glm::vec3(0.64f, -0.50f, -3.41f)},
        {glm::vec3(3.47f, -0.50f, -1.69f)},
        {glm::vec3(2.53f, -0.50f, -4.23f)},
        {glm::vec3(-2.67f, -0.50f, -4.26f)},
        {glm::vec3(-3.80f, -0.50f, 3.49f)},
        {glm::vec3(0.96f, -0.50f, 1.04f)},
    };

    std::vector<float> angles;
    srand(1);

    for(int i = 0; i < 10; i++) {
        windowTransforms[i].mRotation = glm::angleAxis((float)rand()/RAND_MAX*3, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    std::vector<Vertex> screenVertices = {
        {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
    };

    std::vector<unsigned int> screenIndices = {
        0, 1, 2,
        1, 3, 2
    };

    // glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, -5.0f);
    // glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, -3.0f);
    // glm::vec3 lightScale = glm::vec3(0.2f);
    Texture container("assets/textures/container.jpg");
    Texture concrete("assets/textures/concrete.jpg");
    Texture windowTexture("assets/textures/window.png");

    Mesh cube = Mesh(cubeVertices, cubeIndices);
    Mesh billboard = Mesh(billboardVertices, billboardIndicies);
    Mesh screen = Mesh(screenVertices, screenIndices);
    billboard.textures["Albedo"] = &windowTexture;
    
    GameObject box1 = GameObject(cube, Transform(glm::vec3(1.0f, -0.5f, -1.5f)));
    GameObject box2 = GameObject(cube, Transform(glm::vec3(-1.5f, -0.5f, -2.0f)));
    GameObject billboardObject = GameObject(billboard);
    GameObject floor = GameObject(cube, Transform(glm::vec3(0.0f, -1.5f, 0.0f), glm::quat(1.0f, glm::vec3(0.0f)), glm::vec3(10.0f, 1.0f, 10.0f)));
    
    Shader unlitShader = Shader("shader/vertex/default.shader", "shader/fragment/unlit.shader");
    Shader screenShader = Shader("shader/vertex/framebuffer.shader", "shader/fragment/framebuffer.shader");
    Shader skyboxShader = Shader("shader/vertex/skybox.shader", "shader/fragment/skybox.shader");

    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    auto distanceSorter = [](Transform transform1, Transform transform2) {
        float distance1 = glm::length(camera.position - transform1.mPosition);
        float distance2 = glm::length(camera.position - transform2.mPosition);

        return distance1 - distance2 > 0.0f;
    };

    unsigned int cubeMapTexture;
    glGenTextures(1, &cubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    std::vector<std::string> texture_faces = {
        "assets/textures/right.jpg",
        "assets/textures/left.jpg",
        "assets/textures/top.jpg",
        "assets/textures/bottom.jpg",
        "assets/textures/front.jpg",
        "assets/textures/back.jpg",
    };

    int width, height, nrChannels;
    unsigned char *data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    stbi_set_flip_vertically_on_load(false);
    for(unsigned int i = 0; i < texture_faces.size(); i++) {
        data = stbi_load(texture_faces[i].c_str(), &width, &height, &nrChannels, 3);
        if(!data) {
            std::cout << "Failed to read cube map texture " << texture_faces[i] << std::endl;
            stbi_image_free(data);
        } else {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }
    stbi_set_flip_vertically_on_load(true);
    
    while(!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        process_input(window);
        
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(80.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);

        transtack::projectionMatrix = projection;
        transtack::viewMatrix = view;
        
        cube.textures["Albedo"] = &container;
        box1.draw(unlitShader);
        box2.draw(unlitShader);
        
        cube.textures["Albedo"] = &concrete;
        floor.draw(unlitShader);

        std::sort(windowTransforms.begin(), windowTransforms.end(), distanceSorter);
        
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use_shader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        skyboxShader.setMat4f("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4f("projection", projection);
        skyboxShader.setInt("skybox", 0);
        cube.draw(skyboxShader);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        
        for(auto vegetationTransform : windowTransforms) {
            billboardObject.mTransform = vegetationTransform;
            billboardObject.draw(unlitShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
