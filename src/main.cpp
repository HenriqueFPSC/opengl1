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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Open GL 1", NULL, NULL);
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

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");
//    Shader shader2("../assets/vertex_core.glsl", "../assets/fragment_core2.glsl");

    ///@formatter:off
    // Vertex Array
    float vertices[] = {
            //      Positions           Colors              Texture coordinates
            -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.5f,       0.0f, 0.0f, // Bottom Left
            -0.5f,  0.5f, 0.0f,     0.5f, 1.0f, 0.75f,      0.0f, 1.0f, // Top Left
             0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,       1.0f, 0.0f, // Bottom Right
             0.5f,  0.5f, 0.0f,     1.0f, 0.2f, 1.0f,       1.0f, 1.0f, // Top Right
    };
    unsigned indices[] = {
            0, 1, 2, // First Triangle
            3, 1, 2, // Second Triangle
    };
    ///@formatter:on


    // VAO, VBO & EBO
    unsigned VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set Attribute Pointers
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

//    glm::mat4 trans(1.0f);
//    trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0, 0, 1));
//    shader.activate();
    shader.setMat4("transform", transform);

    mainJ.update();
    if (mainJ.isPresent()) {
        std::cout << mainJ.getName() << " is present." << std::endl;
    } else {
        std::cout << "Joystick is not present." << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);
        shader.setFloat("mixVal", mixVal);

        // Render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

//        trans = glm::rotate(trans, glm::radians((float) glfwGetTime() / 20.0f), glm::vec3(0, 0, 1));
        shader.activate();
        shader.setMat4("transform", transform);
//
//        trans2 = glm::rotate(trans2, glm::radians((float) (glfwGetTime() * glfwGetTime()) / -40.0f),
//                             glm::vec3(0, 0, 1));
//        shader2.activate();
//        shader2.setMat4("transform", trans2);

        // Draw Shapes
        glBindVertexArray(VAO);

        // First Triangle
        shader.activate();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Second Triangle
        //shader2.activate();
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *) (3 * sizeof(GLuint)));

        // Gets the new window's buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
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


    // Translate using WASDs
    const float wasdChange = 0.02f;
    if (Keyboard::key(GLFW_KEY_W))
        transform = glm::translate(transform, glm::vec3(0, wasdChange, 0));
    if (Keyboard::key(GLFW_KEY_S))
        transform = glm::translate(transform, glm::vec3(0, -wasdChange, 0));
    if (Keyboard::key(GLFW_KEY_A))
        transform = glm::translate(transform, glm::vec3(-wasdChange, 0, 0));
    if (Keyboard::key(GLFW_KEY_D))
        transform = glm::translate(transform, glm::vec3(wasdChange, 0, 0));

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
