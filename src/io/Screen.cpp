
#include "Screen.h"
#include "Keyboard.h"
#include "Mouse.h"

unsigned int Screen::SCREEN_WIDTH = 800;
unsigned int Screen::SCREEN_HEIGHT = 600;

void Screen::framebufferSizeCallBack(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}

Screen::Screen() {
    window = nullptr;
}

bool Screen::init() {
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Open GL 1", NULL, NULL);
    if (!window) {
        // Window Wasn't Created
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

void Screen::setParameteres() {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Resize CallBack
    glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallBack);

    // Keyboard Callback
    glfwSetKeyCallback(window, Keyboard::keyCallback);

    // Mouse Input Mode and Callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    glEnable(GL_DEPTH_TEST);
}

void Screen::update() {
    glClearColor(0.2f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Screen::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(window, shouldClose);
}