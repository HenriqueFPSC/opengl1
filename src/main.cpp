#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb.cpp>
#include "Shader.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

float mixVal = 0.5f;

glm::mat4 transform(1);
Joystick mainJ(0);

unsigned SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
float x, y, z;
float fov = 45.0f;

int main() {
    std::cout << "Running!" << std::endl;

    int success;
    char infoLog[512];

    // Initialize GLFW
    glfwInit();

    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Open GL 1", NULL, NULL);
    if (window == NULL) { // Could not create window
        std::cout << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    glEnable(GL_DEPTH_TEST);

    // ---- Shaders ---- //
    Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");
//    Shader shader2("../assets/vertex_core.glsl", "../assets/fragment_core2.glsl");

    ///@formatter:off
    // Vertex Array
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    ///@formatter:on


    // VAO, VBO & EBO
    unsigned VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set Attribute Pointers
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    // Texture Coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ---- Textures ---- //
    unsigned texture1, texture2;
    // Texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Load Image
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../assets/img1.jpg", &width, &height, &nChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);

    // Texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load("../assets/img2.png", &width, &height, &nChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);

    shader.activate();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    mainJ.update();
    if (mainJ.isPresent()) {
        std::cout << mainJ.getName() << " is present." << std::endl;
    } else {
        std::cout << "Joystick is not present." << std::endl;
    }

    x = 0;
    y = 0;
    z = 3;

    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Create Transformation for Screen
        glm::mat4 model(1);
        glm::mat4 view(1);
        glm::mat4 proj(1);

        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
        view = glm::translate(view, glm::vec3(-x, -y, -z));
        proj = glm::perspective(glm::radians(fov), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);

        // Set Uniform Variables
        shader.activate();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", proj);
        shader.setFloat("mixVal", mixVal);

        // Render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Draw Shapes
        glBindVertexArray(VAO);

        // Cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Gets the new window's buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}

void processInput(GLFWwindow *window) {
    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    // Change Mix Value with Arrow Keys
    const float mixChange = 0.02f;
    if (Keyboard::key(GLFW_KEY_RIGHT)) {
        mixVal += mixChange;
        if (mixVal > 1) mixVal = 1.0f;
    }
    if (Keyboard::key(GLFW_KEY_LEFT)) {
        mixVal -= mixChange;
        if (mixVal < 0) mixVal = 0.0f;
    }

    // Scale with Arrow Keys
    const float scaleChange = 0.02f;
    if (Keyboard::key(GLFW_KEY_UP)) {
        transform = glm::scale(transform, glm::vec3(1 + scaleChange, 1 + scaleChange, 0));
    }
    if (Keyboard::key(GLFW_KEY_DOWN)) {
        transform = glm::scale(transform, glm::vec3(1 - scaleChange, 1 - scaleChange, 0));
    }


    // Change Camara Position using WASDs
    const float wasdChange = 0.02f;
    const float scrollMultiplier = 0.8f;
    if (Keyboard::key(GLFW_KEY_W))
        y += wasdChange;
    if (Keyboard::key(GLFW_KEY_S))
        y -= wasdChange;
    if (Keyboard::key(GLFW_KEY_A))
        x -= wasdChange;
    if (Keyboard::key(GLFW_KEY_D))
        x += wasdChange;
    if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
        z -= wasdChange;
    if (Keyboard::key(GLFW_KEY_LEFT_CONTROL))
        z += wasdChange;
    fov -= (float) Mouse::getScrollDx() * scrollMultiplier;


    // Translate using Joystick
    mainJ.update();
    const float mainDeadzone = 0.20f;
    const float mainMultiplier = 0.02f;
    float lx = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
    float ly = -mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);

    if (std::abs(lx) > mainDeadzone) {
        transform = glm::translate(transform, glm::vec3(lx * mainMultiplier, 0, 0));
    }
    if (std::abs(ly) > mainDeadzone) {
        transform = glm::translate(transform, glm::vec3(0, ly * mainMultiplier, 0));
    }

    // Scale using Joystick
    const float triggerDeadZone = 0.2;
    const float triggerMultiplier = 0.02f;
    // COMMENTED BECAUSE MY CONTROLLER IS BROKEN
    //float rt = mainJ.axesState(GLFW_JOYSTICK_AXES_RIGHT_TRIGGER);
    float lt = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_TRIGGER);
    //if (std::abs(rt) > triggerDeadZone) {
    //    transform = glm::scale(transform, glm::vec3(1 + rt * triggerMultiplier, 1 + rt * triggerMultiplier, 0));
    //}
    if (std::abs(lt) > triggerDeadZone) {
        transform = glm::scale(transform, glm::vec3(1 + lt * triggerMultiplier, 1 + lt * triggerMultiplier, 0));
    }
}
