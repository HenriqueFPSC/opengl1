#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/models/Cube.hpp"
#include "graphics/models/Lamp.hpp"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"
#include "io/Camera.h"
#include "io/Screen.h"


void processInput(double dt);

Screen screen;

float mixVal = 0.5f;

// glm::mat4 transform(1);
Joystick mainJ(0);

unsigned SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

Camera cameras[]{
        {glm::vec3(0, 0, 3)},
        {glm::vec3(10, 10, 10)}
};
const unsigned camAmount = 2;
unsigned activeCam = 0;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

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

    if (!screen.init()) {
        std::cout << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return -1;
    }

    screen.setParameteres();

    // ---- Shaders ---- //
    Shader shader("../assets/object.vs", "../assets/object.fs");
    Shader lampShader("../assets/object.vs", "../assets/lamp.fs");

    Cube cube(Material::mix(Material::gold, Material::emerald), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
    cube.init();

    Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-1.0f, -0.5f, -0.5f), glm::vec3(0.25f));
    lamp.init();

    mainJ.update();
    if (mainJ.isPresent()) {
        std::cout << mainJ.getName() << " is present." << std::endl;
    } else {
        std::cout << "Joystick is not present." << std::endl;
    }

    while (!screen.shouldClose()) {
        // Calculate Delta Time
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        // Process input
        processInput(deltaTime);

        // Render
        screen.update();

        shader.activate();
        shader.set3Float("light.position", lamp.pos);
        shader.set3Float("viewPos", cameras[activeCam].cameraPos);

        shader.set3Float("light.ambient", lamp.ambient);
        shader.set3Float("light.diffuse", lamp.diffuse);
        shader.set3Float("light.specular", lamp.specular);

        // Create Transformation for Screen
        glm::mat4 view(1);
        glm::mat4 proj(1);

        view = cameras[activeCam].getViewMatrix();
        proj = glm::perspective(glm::radians(cameras[activeCam].getFOV()), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
                                0.1f, 100.0f);

        // Set Uniform Variables
        shader.setMat4("view", view);
        shader.setMat4("projection", proj);

        cube.render(shader);

        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", proj);
        lamp.render(lampShader);

        screen.newFrame();
    }

    cube.cleanup();
    lamp.cleanup();

    glfwTerminate();
    return 0;
}

void processInput(double dt) {
    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        screen.setShouldClose(true);
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

    if (Keyboard::keyWentDown(GLFW_KEY_TAB)) {
        activeCam = (activeCam + 1) % camAmount;
    }

    // Move Camera
    if (Keyboard::key(GLFW_KEY_W))
        cameras[activeCam].updateCameraPosition(CameraDirection::FORWARD, dt);
    if (Keyboard::key(GLFW_KEY_S))
        cameras[activeCam].updateCameraPosition(CameraDirection::BACKWARD, dt);
    if (Keyboard::key(GLFW_KEY_D))
        cameras[activeCam].updateCameraPosition(CameraDirection::RIGHT, dt);
    if (Keyboard::key(GLFW_KEY_A))
        cameras[activeCam].updateCameraPosition(CameraDirection::LEFT, dt);
    if (Keyboard::key(GLFW_KEY_SPACE))
        cameras[activeCam].updateCameraPosition(CameraDirection::UP, dt);
    if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
        cameras[activeCam].updateCameraPosition(CameraDirection::DOWN, dt);

    // Change Camera Direction and FOV
    const float mouseSens = 0.2f;
    cameras[activeCam].updateCameraDirection(Mouse::getDx() * mouseSens, Mouse::getDy() * mouseSens);
    cameras[activeCam].updateCameraFOV(Mouse::getScrollDx());
}
